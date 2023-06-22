project('crosslibs', 'c', 
	license : 'Apache-2.0' ,
	license_files : ['LICENSE'] ,
	default_options : [
		'warning_level=3'
		'c_std=c11',
		
#		'buildtype=release',
		'buildtype=debugoptimized',
		'default_library=both',								# library targets build shared and static libraries
		'prefer_static=true',								# Whether to try static linking before shared linking
		'vsenv=false',									# Default false. Activate Visual Studio environment
		'b_vscrt=static_from_buildtype'		
	]
)

# These are global compiler argements - dependend on used compiler - can't overwrite config time things
#add_global_arguments( '-Dbuildtype=' + get_option('BUILD_CROSSLIBS_TYPE'), language : 'c')
#add_global_arguments( '-Dvsenv=' + get_option('BUILD_CROSSLIBS_FORCE_MSVC').to_string(), language : 'c')

add_project_arguments( '-Dbuildtype=' + get_option('BUILD_CROSSLIBS_TYPE'), language : 'c')
#add_project_arguments( '-Dvsenv=' + get_option('BUILD_CROSSLIBS_FORCE_MSVC').to_string(), language : 'c')



thread_dep = dependency('threads')

crosslibs_incdir = include_directories('include')


libcrosstimer = library ('crosstimer',
			 'src/crosstimer.c',
	  		 include_directories : crosslibs_incdir,
			 version: '0.9.1', 
			 soversion : '0',
			 dependencies : thread_dep,
			 install : false)

libcrosstimer_dep = declare_dependency(include_directories : crosslibs_incdir, link_with : libcrosstimer.get_static_lib())
libcrosstimer_sodep = declare_dependency(include_directories : crosslibs_incdir ,link_with : libcrosstimer.get_shared_lib())

subdir( 'example')
