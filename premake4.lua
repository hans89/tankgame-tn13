-- A solution contains projects, and defines the available configurations
solution "tankgame-tn13"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "tankgame-tn13"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.cpp", "**.c", "**.hpp" }
      targetdir "bin"
      defines { "cimg_use_png" }

      configuration "windows"
         -- buildoptions { "/MD" }
         links { "user32", "gdi32", "shell32", "libpng16", "zlib" }
         includedirs { "lib/win32/include" }
         libdirs { "lib/win32" }
 
      configuration "linux"
         links { "pthread", "X11", "png", "z" }
         includedirs { "/usr/X11R6/include" }
         libdirs { "/usr/X11R6/lib" }
    
      configuration "macosx"
      -- OS X frameworks need the extension to be handled properly
         links { "m", "pthread", "X11" , "png", "z"}
         includedirs { "/usr/X11R6/include" }
         libdirs { "/usr/X11R6/lib" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }