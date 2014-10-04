-- A solution contains projects, and defines the available configurations
solution "tankgame-tn13"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "tankgame-tn13"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.cpp", "**.c", "**.hpp" }
      targetdir "bin"
      
      configuration "windows"
         -- buildoptions { "/MD" }
         defines { "cimg_use_png" }
         links { "user32", "gdi32", "shell32", "libpng16", "zlib" }
         includedirs { "lib/win32/include" }
         libdirs { "lib/win32" }
 
      configuration "linux"
         links { "pthread", "X11" }
         includedirs { "/usr/X11R6/include" }
    
      configuration "macosx"
      -- OS X frameworks need the extension to be handled properly
         defines { "cimg_use_png" }
         links { "m", "pthread", "X11" , "png", "z"}
         includedirs { "/usr/X11R6/include" }
         libdirs { "/usr/X11R6/lib" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }