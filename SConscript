
Import( 'env' )

boostLibs = ('system', 'program_options', 'filesystem/v3', )

boostSources = [ Glob( 'libs/%s/src/*.cpp' % lib ) for lib in boostLibs ]

boostSources += [ Glob( 'libs/thread/src/pthread/*.cpp' ) ]

boost = env.Library( 'boost', boostSources, CPPPATH='.' )

Return( 'boost' )
