{
  "targets": [
    {
      "target_name": "GRSDK",
      "sources": [ "GRSDK.cpp", "./src/grDevManager.cpp" ],
      "ldflags": [ "-lbluetooth", "-std=c++11", "-pthread", "-lncurses", "-ltinfo", "-lstdc++" ]
    }
  ]
}
