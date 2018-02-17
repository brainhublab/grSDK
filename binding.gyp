{
	"targets": [
		{
			"includes": [
				"auto.gypi"
			],
			"sources": [
				"src/grDevManager.cpp",
				"src/grDevice.cpp",
        "src/grConnection.cpp"
			],
      "ldflags": [
        "-lbluetooth", "-std=c++11", "-pthread", "-lncurses", "-ltinfo", "-lstdc++"
      ],
      "include_dirs": [
        "inc"
      ]
		}
	],
	"includes": [
		"auto-top.gypi"
	]
}
