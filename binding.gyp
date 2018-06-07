{
	"targets": [
		{
			"includes": [
				"auto.gypi"
			],
			"sources": [
				"src/grDevManager.cpp",
				"src/grDevice.cpp",
        "src/grConnection.cpp",
        "src/grMadgwick.cpp",
        "src/grGrt.cpp",
        "src/grAlgorithm.cpp"
			],
      "ldflags": [
        "-lbluetooth", "-std=c++11", "-pthread", "-lncurses", "-ltinfo", "-lstdc++", "-L./externAssets/grt/lib"
      ],
      "include_dirs": [
        "inc", "externAssets/grt/include", "externAssets/eigen"
      ],
      'cflags_cc!': [ '-fno-rtti' ]
		}
	],
	"includes": [
		"auto-top.gypi"
	]
}
