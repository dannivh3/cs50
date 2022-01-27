SELECT name
FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE stars.movie_id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE name LIKE "Kevin Bacon" AND birth = 1958))
AND name != "Kevin Bacon";




