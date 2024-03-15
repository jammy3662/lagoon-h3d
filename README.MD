## Lagoon
### Perfect your aim, pick off enemies, and paint the world your color!

A team-based online paint shooter game. Join friends in several exciting game modes; take your pick from a selection of unique weapons; earn rewards and customizations. Perfect your aim, pick off enemies, and paint the world your color!

### Structure

The intention is that the code is compiled to LLVM bitcode, which is platform independent. It allows the code to be compiled on the dev's system and shipped to users as a package which is mostly self-contained.
Game resources will be contained in some kind of binary archive format. The tape archive format Tar is a good candidate, if portability and byte order can be addressed.

### H3D

Horde (Horde3D) is a rendering engine focused on interfacing directly with OpenGL's functionality, utilizing specialized shaders and a programmable pipeline.
Its built-in resource management system makes it convenient for this kind of project as well. Modding and local customizations are easy to create, while staying out of the way to leave resource creation as a separate process for the artist(s).
Initially, the idea was to write a custom, in-house rendering toolkit. I started writing that toolkit multiple times, and its current iteration is called LRE (Lagoon Rendering Engine). However, since Horde does all the tasks which I wanted to implement for LRE, the simpler choice is to use Horde and focus on the rendering pipeline itself. 
