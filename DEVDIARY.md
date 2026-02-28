# February 25, 2026
**3 Hours**

Started the project. Using my ZaniJS as a jumping off point, but intend to fix the mistakes made, including
- Using JS
- Synchronous file loading
- 1 file per entry
- Abysmal getCollection loading all into memory
- B-Tree had issue with hitting disk too many times.

**Accomplished**
Set up the project, WSL, and outlined basic logging functionality architecture. 

# February 26, 2026
**4 Hours**
Today was entirely setting up architecture and figuring out project scope. Very little to no actual code was written, and what was written is pseudocode and thus, unusable outside theories. However, it should direct later development smoothly and ensure the right foot is stepped off with.

# February 27, 2026
**9 Hours**
Lots of progress, primarily learning C++ is exactly nothing like I've used it for single file projects. Likewise, had to update the makefile to match a new subfolder directory.

**Accomplished**
- Update makefile for subdirectories
- Fully implemented a multi-thread, asynchronous logger, and a console sink.
- Lost a fight to mutex.

**Todo**
- Add back pressure to logger
- Add a sink protection.