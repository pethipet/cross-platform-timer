project('libcrosstimer', 'c', 
	license : 'Apache-2.0' ,
	license_files : ['LICENSE'] ,
	default_options : [
		'buildtype=debugoptimized',
		'default_library=both',
		'c_std=c11',
#		'prefer_static=true'
	]
)


thread_dep = dependency('threads')



inc = include_directories('include')
libcrosstimer = library ('crosstimer',
			 'src/crosstimer.c',
	  		 include_directories : inc,
			 version: '0.9.1', 
			 soversion : '0',
			 dependencies : thread_dep,
			 install : false)

libcrosstimer_dep = declare_dependency(include_directories : inc, link_with : libcrosstimer)



subdir( 'example')





