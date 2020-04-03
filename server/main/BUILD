cc_binary (
	name = "userLoginManagerService",
	srcs = ["main.cpp"],
	copts = [
		"-Imylib/myLibIni",
		"-Iserver/service",
		"-Iexternal/usr/local/include",
		"-Iexternal/usr/local/mysql/include",
	],
	linkopts = [
		"-L/usr/local/lib",
		"-lmysqlcppconn",
	],
	deps = [
		"//mylib/myLibIni:myLibIni",
		"//server/service:userLoginManageService",
	],

)
