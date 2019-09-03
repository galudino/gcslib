gcslib by Gemuele Aludino - a container library for C (C99)
--------------------------------------------------------------------------------

gcslib will be actively tested on:
    macOS
    CentOS Linux
 
TO USE GCSLIB:
Clone this repository (download it to your machine)

The provided Makefile will recognize files in the include and src directories,
and the object files will reside in the build directory.

A test client is provided for your convenience (and for my testing purposes),
the test client is named gcslib.c and is located in the client directory.
You can run the test client by running the gcslib in the root of the repo directory.

Consult the Makefile to review the required compile flags if you wish to
extract certain containers out of gcslib for use in your projects.

All gcslib containers depend on the following files:
utils.h
utils.c
iterator.h
iterator.c

Sequential containers will also depend on
mergesort.h
mergesort.c

Templated sequential containers will depend on
mergesort_template.h
mergesort_template.c

You can review the dependencies for each container by consulting gcslib.h.
gcslib.h is a catch-all header for the entire library, if you wish to use
everything at once. Otherwise, you can omit what you don't need for your project,
so as long as a container's dependency requirements are met.

--------------------------------------------------------------------------------
CHANGELOG

09/03/2019
vector (void *) and vector(T) are ready to use.
gcslib repository online.

--------------------------------------------------------------------------------
Author's note:
gcslib stands for "Gem's Computer Science library",
and it is a container library for C (specifically, C99).

It is loosely modeled off of the C++ standard library containers.

gcslib was mainly a learning exercise for me;
I could brush up on my data structures,
while at the same time, also figure out how to produce
(somewhat) object-oriented code in a language
that does not readily support the object-oriented paradigm.

However, I wanted to stay true to C's syntax (for the most part),
and not stray too far from what is conventional/expected.
I've seen some pretty macro-heavy stuff online,
and although those solutions do achieve the desired results
(object orientation in C) -- the source code hardly resembles C.

I had ultimately decided on sticking to procedural code,
while still taking some cues from OOP, such as encapsulation
(through opaque structs).

The downside, however, is that containers
are not able to be allocated by automatic storage (on the stack) --
they must always be created/instantiated using an associated "new" function
for a specific container type. I am still debating as to whether
opaque structs are worth this tradeoff.

Each container will have an iterator associated with it.
After many iterations (no pun intended), I had decided to make
iterators non-opaque, since they can become invalidated, and freeing
invalidated iterators may become a nuisance to the user.

Besides, the iterators already consist of the container pointer variable
that was used to instantiate the iterator in the first place, and the client
has access to that container variable on their end.

To the client, there is only one iterator type,
but the behavior of the iterator is dictated by the vtable
(virtual function table) -- it is initialized with when calling a
"_begin" or "_end" function for a particular container.

The iterator, just as in C++, becomes the common interface between
all the different kinds of containers.

Files in the _debug folder are containers still in the testing phase,
or are being redone (I'm keeping them around for now so I can still use
some of the algorithms I've written for them) -- the files I've left
in the include and src folders are ready to use, and will be picked up by the
provided Makefile.

If you want to pull out any container from gcslib
to use for your project, be sure to get:
    - (container_name).h/(container_name).c
    - utils.h/utils.c
    - iterator.h/iterator.c

Sequential structures will also require
    - mergesort.h/mergesort.c

Eventually, gcslib will have the following containers:

    vector      dynamically-sized array
    list        doubly-linked list
    slist       singly-linked list
    rbtree      left-leaning red-black tree
    set         associative structure that uses rbtree
    pair        dual-element tuple
    map         associative structure that uses an rbtree of pair

Each of these containers will use a (void *) to store their data.

There will also be a second category of containers, just like the ones above,
but "templated".

These "templates" use preprocessor token-pasting to generate
type-safe containers. Not all of the code is generated using macros --
just the portions of the code that contain type information are macros.

I believe the templated containers will be a better option
for integral data types, like int8_t (char) or int32_t (int),
and will have a friendlier API that (in my opinion) is easier to use.

Template "instantiations" of
    int types (char, short, int, long long, both signed/unsigned)
    float types (float, double, long double)
    string types (char *, const char *)
will be included with gcslib, so that you don't have to make them.

They will also serve as an example so that when you want to make a template
instantiation for your own type, you can easily do so.
