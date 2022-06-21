
import os

from conan import ConanFile
from conan.tools.gnu import AutotoolsToolchain, Autotools
from conans import ConanFile, AutoToolsBuildEnvironment
from conan.tools.layout import basic_layout
from conan.tools.files import chdir


class OrbitsConan(ConanFile):
    name = "orbits"
    version = "0.1"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Orbits here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    requires = "boost/1.79.0"
    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "configure.ac", "Makefile.am", "src/*"

    def layout(self):
        basic_layout(self)

    def generate(self):
        at_toolchain = AutotoolsToolchain(self)
        at_toolchain.generate()

    def build(self):
        env_build = AutoToolsBuildEnvironment(self)
        autotools = Autotools(self)

        CXXFLAGS=""
        for PATH in  env_build.include_paths:
            incdir = " -I%s"%PATH
            CXXFLAGS = CXXFLAGS + incdir
        os.environ["CXXFLAGS"] = CXXFLAGS
        
        autotools.autoreconf()
        autotools.configure()
        autotools.make()

    def package(self):
        autotools = Autotools(self)
        autotools.install()
        

    def build_requirements(self):
        self.tool_requires("boost/1.79.0")
#        self.test_requires("freeglut/3.2.1")

