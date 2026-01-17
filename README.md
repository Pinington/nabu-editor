```
        "gilga.msh inc.
         _________           ___    ___            ________         ___       __  
       /"         `\\      /   \\  /   \\         /         \\    //  \\\\   /   \\ 
      //    ________/     /     \\/     \\      //    ..-.___\\   '      \\_/     . 
     /    /  ______       ;              \\     /    //    \"\"   |               |
    \\    \ |____  \    /    /\\    /\\   \\   \\    \\    ___    |       _       |
     \\    \____/   /  /    /  \\__/  \\   \\   \\    '--'   //   '      / \\     ' 
      \\ __________/   \\__/          \\___/     \\_________/      \\___/   \\___/  
```

Nabu Text Editor is a lightweight C-based text editor with minimal autocorrect features.
The terminal UI is implemented using the SDL2 library, and the autocorrect system uses Levenshtein distance combined with a set of known words.
This personal project started two weeks before its initial GitHub push.

# REQUIREMENTS
-gcc or clang  
-SDL2  
-SDL2_ttf  

# Make
regular make

# USAGE
Not yet implemented:
    text_editor.exe <filename.txt> to save writing into a file
The long run idea is:
    text_editor.exe, and use windows/linux file system API to select a folder / text file