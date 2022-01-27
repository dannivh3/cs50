import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    userID = session.get("user_id")
    if request.method == "GET":
        try:
            getStocks = db.execute("SELECT * FROM shares WHERE user_id = ?",userID)
            getUser = db.execute("SELECT * FROM users WHERE id = ?",userID)

            totalPrice = 0
            totalList = {}

            for getStock in getStocks:
                print(getStock)
                print("price: ",getStock["price"])
                print("quantity: ",getStock["quantity"])
                if getStock["type"] == "BOUGHT":
                    total = int(getStock["price"] * getStock["quantity"])
                else:
                    total = int(getStock["price"] * -getStock["quantity"])
                print("total: ",total)

                totalPrice += total
                print (totalPrice)
                symbol = getStock["symbol"]
                if symbol not in totalList:
                    totalList[symbol] = {"name": getStock["name"], "total": total, "quantity": getStock["quantity"], "price": getStock["price"]}
                else:
                    totalList[symbol]["total"] += total
                    totalList[symbol]["quantity"] += getStock["quantity"]
            deletion = []
            for key, value in totalList.items():
                if value["quantity"] == 0:
                    deletion.append(key)
                value["price"] = usd(value["price"])
                value["total"] = usd(value["total"])
            for foo in deletion:
                totalList.pop(foo, None)


            cash = getUser[0]["cash"]
            totalCash = usd(cash + totalPrice)
            cash = usd(cash)


            return render_template("index.html",stocks=totalList, cash=cash, total=totalCash)

        except RuntimeError:
            return apology("No shares found")
    else:
        getUser = db.execute("SELECT * FROM users WHERE id = ?",userID)
        getDep = float(request.form.get("deposit"))
        cash = float(getUser[0]["cash"])
        newCash = getDep + cash
        db.execute("UPDATE users SET cash = ?",newCash)
        return redirect("/")
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        getSymbol = request.form.get("symbol")
        compInfo = lookup(getSymbol)
        if not compInfo:
            return apology("No company found")
        getShares = int(request.form.get("shares"))
        if getShares <= 0:
            return apology("Shares need to be a positive integer")

        pricePerShare = float(compInfo["price"])
        totalPrice = pricePerShare * getShares

        userID = session.get("user_id")
        currCash = db.execute("SELECT cash FROM users WHERE id = ?", userID)
        currCash = currCash[0]["cash"]
        print(totalPrice)
        print(currCash)
        if currCash < totalPrice:
            return apology("Not enough cash")
        now = datetime.now()
        newCash = currCash - totalPrice
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, userID)
        db.execute("CREATE TABLE IF NOT EXISTS shares (shares_id INTEGER PRIMARY KEY, name TEXT NOT NULL, symbol TEXT NOT NULL, type TEXT NOT NULL, price NUMERIC NOT NULL, quantity INTEGER NOT NULL, date TEXT NOT NULL, user_id INTEGER NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id))")

        db.execute("INSERT INTO shares (user_id, name, symbol, type, price, quantity, date) VALUES (?, ?, ?, ?, ?, ?, ?)",userID, compInfo["name"], compInfo["symbol"], "BOUGHT", pricePerShare, getShares, now.strftime("%d/%m/%Y %H:%M:%S"))

        return redirect("/")
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userID = session.get("user_id")
    try:
        getStocks = db.execute("SELECT * FROM shares WHERE user_id = ?",userID)
    except:
        return apology("Couldnt find history")


    return render_template("history.html",stocks=getStocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):

            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        quotes = lookup(symbol)
        if quotes:
            quotes["price"] = usd(quotes["price"])
            return render_template("/quoted.html", quotes=quotes)
        else:
            return apology("No company found")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("/register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        rpassword = request.form.get("r-password")

        if password != rpassword:
            return apology("Password's do not match")
        rows = db.execute("SELECT username FROM users WHERE username = ?", username)
        hpassword = generate_password_hash(password)
        if len(rows) != 0:
            return apology("Username already exist")

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hpassword)
        return render_template("login.html")
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():


    userID = session.get("user_id")
    getStocks = db.execute("SELECT * FROM shares WHERE user_id = ?",userID)
    getUser = db.execute("SELECT * FROM users WHERE id = ?",userID)

    if request.method == "POST":
        formQuantity = int(request.form.get("quantity"))
        formSymbol = request.form.get("symbol")
        compInfo = lookup(formSymbol)

        getQuantity = db.execute("SELECT SUM(quantity) FROM shares WHERE user_id = ? AND symbol = ?",userID, formSymbol)
        print(getQuantity[0]["SUM(quantity)"])
        realQuantity = int(getQuantity[0]["SUM(quantity)"])

        if formQuantity > realQuantity:
            return apology("Can't sell more than you own")
        elif formQuantity <= realQuantity:
            getPrice = db.execute("SELECT price FROM shares WHERE user_id = ? AND symbol = ?", userID, formSymbol)
            realPrice = getPrice[0]["price"]
            getCash = db.execute("SELECT cash FROM users WHERE id = ?", userID,)
            realCash = getCash[0]["cash"]

            totalPrice = realPrice * formQuantity
            newCash = realCash + totalPrice
            db.execute("UPDATE users SET cash = ?", newCash)
            now = datetime.now()
            db.execute("INSERT INTO shares (user_id, name, symbol, type, price, quantity, date) VALUES (?, ?, ?, ?, ?, ?, ?)",userID, compInfo["name"], compInfo["symbol"], "SOLD", -realPrice, -formQuantity, now.strftime("%d/%m/%Y %H:%M:%S"))

        return redirect("/")

    else:

        totalList = []
        for getStock in getStocks:
            symbol = getStock["symbol"]
            if symbol not in totalList:
                totalList.append(symbol)

        return render_template("sell.html",stocks=totalList)
