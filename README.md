# WordleBot

A little program that helps solve Wordle. This was one of my first personal projects in C++, so the code is pretty rough in places, but it works.

## How it works

You give it a guess and the colors Wordle gave back. It cuts down the list of possible answers, then picks the next guess that should leave the fewest words on average.

Working out the best guess can be slow, so every answer gets saved to `database.csv`. The next time the same guess and feedback come up, it just looks it up.

## Files

- `WordleBot.cpp` - all the code
- `WordleList.csv` - the list of possible Wordle answers
- `database.csv` - saved guesses so it doesn't have to redo the work

## Running it

```
g++ WordleBot.cpp -o wordlebot
./wordlebot
```

Run it from the same folder as the CSV files.

Type your first guess and the feedback on one line, like this:

```
slate wwygw
```

Use `g` for green, `y` for yellow, and `w` for gray. It will print the next word to try. Enter that in Wordle, then type the new feedback. Type `done` when you get it.

There's also some code in there for simulating games and testing which starting word does best. It's not hooked up to `main` right now, but you can call `findAverage` or `findBestWord` if you want to try it.
