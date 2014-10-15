-- A solution contains projects, and defines the available configurations
solution "tankgame-tn13"
   configurations {"Debug", "Release"}

   configuration "Debug"
      defines "DEBUG" 
      flags "Symbols"
   
   configuration "Release"
      defines "NDEBUG"
      flags "Optimize"   

   -- A project defines one build target
   project "tankgame-tn13"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.cpp", "**.c", "**.hpp" }
      targetdir "bin"
      defines "cimg_use_png"

      configuration "windows"
         -- buildoptions { "/MD" }
         links { "user32", "gdi32", "shell32", "libpng16", "zlib" }
         includedirs { "lib/win/include" }        
      
      configuration {"windows", "Debug", "vs2010"}
         libdirs { "lib/win/vs2010-debug" }

      configuration {"windows", "Release", "vs2010"}
         libdirs { "lib/win/vs2010-release" }

      configuration {"windows", "Debug", "vs2012"}
         libdirs { "lib/win/vs2012-debug" }

      configuration {"windows", "Release", "vs2012"}
         libdirs { "lib/win/vs2012-release" }

      configuration {"windows", "Debug", "vs2013"}
         libdirs { "lib/win/vs2013-debug" }

      configuration {"windows", "Release", "vs2013"}
         libdirs { "lib/win/vs2013-release" }
      
      configuration "linux"
         links { "pthread", "X11", "png", "z" }
         includedirs { "/usr/X11R6/include" }
         libdirs { "/usr/X11R6/lib" }
    
      configuration "macosx"
      -- OS X frameworks need the extension to be handled properly
         links { "m", "pthread", "X11" , "png", "z"}
         includedirs { "/usr/X11R6/include" }
         libdirs { "/usr/X11R6/lib" }