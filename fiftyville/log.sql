-- Getting the description from crime that happened at said time and place
SELECT description
FROM crime_scene_reports
WHERE day = 28 AND month = 7 AND year = 2021
AND street LIKE "Humphrey Street"
-- The Result:
-- place of theft: Humphrey Street bakery
-- Time of theft: 10:15am
-- all 3 Witnesses mentioned bakery

-- Checking the transcripts from the witnesses
SELECT name, transcript
FROM interviews
WHERE day = 28 AND month = 7 AND year = 2021
-- Result:
-- the thief was seen getting into a car (might want to look at car records bakery_security_logs from "10:15-10:30")
-- Eugene thinks he knows him, also saying he saw him withdraw some money from an ATM on "Leggett Street"
-- The thief was heard planning to take the earliest flight tomorrow morning(day = 29 AND month = 7) but another person pays for the flight.
-- Emma says she is the bakery owner and witness someone come in and whispering into a phone for half an hour(maybe thief was there long before the crime)

-- Checking the security logs to find license plate within the time period. keeping node of what owner said
SELECT hour, minute, license_plate, activity
FROM bakery_security_logs
WHERE day = 28 AND month = 7 AND year = 2021
AND hour = 10 AND minute BETWEEN 15 AND 45
AND activity LIKE "exit"
-- license plate to check:
-- G412CB7, 6P58WS2, 4328GD8, L93JTIZ, 322W7JE, 0NTHK55, 94KL13X, 5P2BI95

-- Checking up on atm transaction matching witness description
SELECT account_number, transaction_type, amount, atm_location
FROM atm_transactions
WHERE day = 28 AND month = 7 AND year = 2021
AND atm_location LIKE "Leggett Street"
AND transaction_type LIKE "withdraw"
-- Results:
-- 28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199

-- checking the earliest flights
SELECT full_name, city, flights.hour, flights.minute
FROM airports
    JOIN flights
    ON flights.origin_airport_id = airports.id
    WHERE flights.day = 29 AND flights.month = 7 AND year = 2021
    AND airports.city LIKE "fiftyville"
ORDER BY flights.hour ASC

-- Checking which calls was made that was less than 60 sec
SELECT *
FROM phone_calls
WHERE day = 28 AND month = 7 AND year = 2021
AND duration < 60

-- Finding the thief with all the evidence i gathered
SELECT * -- getting every detail of this scumbag
FROM people
WHERE phone_number IN
    (SELECT caller      --Phone number clause
    FROM phone_calls
    WHERE day = 28
    AND month = 7 AND year = 2021
    AND duration < 60)
AND passport_number IN  -- Passport clause
    (SELECT passport_number
    FROM passengers
        JOIN flights
        ON flights.id = passengers.flight_id
        JOIN airports
        ON airports.id = flights.origin_airport_id
    WHERE airports.city LIKE "fiftyville"   --the passport has to be going from fiftyville
    AND flights.day = 29 AND flights.month = 7 AND flights.year = 2021  -- On the next day
    AND flights.hour = 8)   -- Earliest flight was 8 am
AND license_plate IN    -- License plate found by checking the exit status on specific time
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND year = 2021 -- on the day of the robbery
    AND hour = 10 AND minute BETWEEN 15 AND 25 -- in a 10 minute window
    AND activity LIKE "exit")
AND id IN   -- id clause
  (SELECT person_id -- Getting the id from the bank accound where i can find when the card was used on the same day
    FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number
        FROM atm_transactions
        WHERE day = 28 AND month = 7 AND year = 2021 -- was last used on this date
        AND atm_location LIKE "Leggett Street" -- and witness told it was on this location
        AND transaction_type LIKE "withdraw")); -- and witness told he withdraw money
-- Bruce is the thief

-- Finding the destination
SELECT city -- getting the city
FROM airports
WHERE id IN
    (SELECT destination_airport_id -- getting the id from a destination airport
    FROM flights
    WHERE id IN
        (SELECT flight_id -- getting the id from the passangers passport
        FROM passengers
        WHERE passport_number IN
            (SELECT passport_number --just getting the passport number from a guy named bruce which is the thief
            FROM people
            WHERE name LIKE "Bruce")));

-- Finding the accomplis
-- Pretty easy search if you know the name of the thief
-- just seeing the phonecalls Bruce made and setting the information we got from witnesses
-- boom found Robin is the accomplies
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE caller IN
        (SELECT phone_number
        FROM people
        WHERE name LIKE "Bruce")
    AND day = 28 AND month = 7 AND year = 2021
    AND duration < 60);
