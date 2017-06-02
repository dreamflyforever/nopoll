env = Environment(CPPPATH = '.')
src = Glob('*.c')
env.Library('foslib',src, CFLAGS = ['-g', '-fcolor-diagnostics', '-Werror'], CC = 'clang')

src = Glob('main/nopoll-regression-client.c')
env.Program('client',
		src,
		LIBS = ['m', 'ssl', 'crypto', 'foslib'],
        	LIBPATH = ['.'], 
        	CCFLAGS = '-DHELLOSCONS',
		CFLAGS = ['-g', '-fcolor-diagnostics', '-fpack-struct', '-fno-strict-aliasing', '-Wno-unused-variable', '-Os', '-Wall'],
		CC = 'clang')

src = Glob('main/nopoll-regression-listener.c')
env.Program('listener',
		src,
		LIBS = ['m', 'ssl', 'crypto', 'foslib'],
        	LIBPATH = ['.'], 
        	CCFLAGS = '-DHELLOSCONS',
		CFLAGS = ['-g', '-fcolor-diagnostics', '-fpack-struct', '-fno-strict-aliasing', '-Wno-unused-variable', '-Os', '-Wall'],
		CC = 'clang')
