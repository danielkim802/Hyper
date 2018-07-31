# Hyper
Hyper is a language I made for fun with the simple goal of minimizing using the shift key as much as possible (and minimizing keystrokes in general), so all syntax (with the exception of arithmetic operators and begin/end braces) can be written shift-free (see cheatsheet.hyp in examples for syntax overview)! The language itself is meant to be a scripting language with highly functional features which are outlined below. Take a look at the examples directory to get a feel for the syntax and some code samples. Note that while the language is more or less functional at this point, it is still a work in progress: the standard library still needs to be written and some more commonly used operators need to be implemented into the language. Hyper is parsed and compiled into bytecode using Python and then executed using a virtual machine written in C.

## Getting started
To build Hyper (you need Python 2.7 and gcc):
```
git clone https://github.com/danielkim802/Hyper.git
cd Hyper
make
```
To run a script:
```
./hyper myScript.hyp
```

## Function declarations
Functions are first-class objects in Hyper, so they are created and assigned to variables just like any other primitives. Functions can be created using the 'fun' keyword. A function call consists of the variable name followed by space-separated arguments.
```
; function declaration with a single argument 'a'
fun a {
  return a + 1
}

; assigning to a variable
var add = fun a b { return a + b }

; calling the add function
add 1 2 ; output: 3
```

## Closures
Functions will hold onto the environment in which they were created, allowing them to do stuff like this:
```
var makeCounter = fun {
  var count = 0
  var counter = fun {
    count += 1
    print count
  }
  return counter
}

var counter = makeCounter null ; note: functions with no arguments can be called with null
counter null ; output: 1
counter null ; output: 2
```
Additionally, a function that is called with an insufficient number of arguments will create a closure with the arguments provided and return a new function.
```
var add = fun a b {
  return a + b
}
var add3 = add 3
print add3 5 ; output: 8
```

## Pipeline operator
The pipeline operator is built into Hyper and can be used as follows:
```
var incr = fun x { return x + 1 }
print 10 \ incr \ incr \ incr ; output: 13
```

## Structs
Since classes are not built into Hyper, structs are meant to be a very simple form of a class. A struct is essentially a block of code whose environment is captured and saved and can be modified at a later time. 
```
; struct declaration
var point = struct {
  var x = 0
  var y = 0

  var hash = fun {
    return x * y
  }
}

print point.hash null ; output: 0
point.x = 5
point.y = 20
print point.hash null ; output: 100
```

## Inline Assembly
Hyper supports inline assembly giving the user control over the virtual machine, but may lead to unpredictable behavior if you don't know what you're doing. Take a look at instructionset.txt for correct arguments for each function.
```
$ load_int 20
$ load_int 30
$ add
$ print
; output: 50

$ store_name hello
$ load_string 'hello world'
$ assign_name hello
print hello ; output: hello world
```
The virtual machine works on a stack-based architecture so the first two lines are pushing 20 and 30 to the stack, 'add' pops two elements off the stack, adds them, and pushes the result onto the stack, and 'print' pops one element and prints it.

