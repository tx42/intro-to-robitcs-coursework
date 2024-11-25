# Robotics coursework

This is a repository for year-2 robotics module.

The repository is divided into 4 folders:

* algorithms: folder for big sketches and algorithms
* lib: folder for shared resources which can be used in multiple sketches
* measures: small sketches, mainly for data gathering
* results: csv's and tsv's of the data gathered

## Creating a sketch

To create a sketch, create a folder either in `algoritms/` or in `measures/`
with the name of your sketch. Inside of that folder create a file with the
same name and `.ino` extension in the end. So if we want to create a sketch `example`
in algorithms folder, the directory structure would be like this:

```text
algorithms/
    example/
        example.ino
    some_other_algo/
    ...
lib/
measures/
results/
```

Alternatively, you could create new a sketch in arduino IDE and save it. Arduino IDE
will create a directory and a file for you so you will then only need to move the folder
in `algorithms/` or `measures/`.

## Using `lib/`

`lib/` folder contains different useful snippets of code. In order
to include them in your sketch just copy all the files from the library you need
into your sketch folder (it's ugly but I can't do better honestly). Then you will be able
to use them using local includes like so:

```C
#include "somefile.h"
// note the quotes ("") instead of anlgled braces (<>)
// also, include only .h or .hpp files, not c/c++
```

For documentation, whether look for `readme.md` or `architecture.md` files inside the lib.

## Compiling, testing and uploading

Open the sketch in arduino IDE, and then build it. Check and resolve any errors you find.
