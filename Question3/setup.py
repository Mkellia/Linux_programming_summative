from setuptools import setup, Extension

module = Extension('q3stats', sources=['Q3.c'])

setup(
    name='q3stats',
    version='1.0',
    description='C Extension for fast statistics',
    ext_modules=[module]
)
