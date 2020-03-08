cc_library(
	name = "serverRespositorySrc",
	srcs = [
		"userPasswordRepository.cpp",
		"userLoginManageRepository.cpp",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Iserver/domain",
		"-Iserver/repository/include",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/domain:serverDomain",
		"//server/repository/include:serverRespository"
	],
	visibility = ["//visibility:public"],
)