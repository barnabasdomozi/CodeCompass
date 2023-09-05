# Setting up CodeCompass on Ubuntu 22.04

### Install packages from standard repositories

```bash
sudo apt install git curl wget cmake make libboost-all-dev \
  g++ g++-9 gcc-11-plugin-dev \
  llvm-11-dev clang-11 libclang-11-dev \
  default-jdk libssl-dev libgraphviz-dev libmagic-dev libgit2-dev exuberant-ctags doxygen \
  libldap2-dev libgtest-dev
```

Database support

```bash
# For SQLite database systems:
sudo apt install libsqlite3-dev

# For PostgreSQL database systems:
sudo apt install postgresql-server-dev-14
```

### Compile LLVM project 10 from source

Download source and extract it
```bash
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/llvm-project-10.0.0.tar.xz
tar -xvf llvm-project-10.0.0.tar.xz
cd llvm-project-10.0.0
```

Building LLVM
```bash
cmake -S llvm -B build -G "Unix Makefiles" \
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lldb;lld;polly" -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_TARGETS_TO_BUILD=X86 -DCMAKE_CXX_STANDARD=17 -DLLVM_ENABLE_RTTI=on
```

```bash
cmake --build build/ -j $(nproc)
```

Install LLVM
```bash
cmake --install build/ --prefix /usr/local  # or somewhere else
```

### Compile ODB from source

Install build2

```bash
wget https://download.build2.org/0.16.0/build2-install-0.16.0.sh
sh build2-install-0.16.0.sh --yes --trust yes "<build2_install_dir>"
```

Building ODB
```bash
export PATH="<build2_install_dir>/bin:$PATH"

# Configuring the build
cd <odb_build_dir>
bpkg create --quiet --jobs $(nproc) cc \
  config.cxx=g++ \
  config.cc.coptions=-O3 \
  config.bin.rpath=<odb_install_dir>/lib \
  config.install.root=<odb_install_dir>

# Getting the source
bpkg add https://pkg.cppget.org/1/beta --trust-yes
bpkg fetch --trust-yes

# Building odb
bpkg build odb --yes
bpkg build libodb --yes
bpkg build libodb-sqlite --yes
bpkg build libodb-pgsql --yes
bpkg install --all --recursive
```

> Note: now you may delete the Build2 toolchain installed in the <build2_install_dir> folder, if you do not need any longer.

### Compile Thrift 13.0 from source

```bash
# Download and uncompress Thrift:
wget "http://www.apache.org/dyn/mirrors/mirrors.cgi?action=download&filename=thrift/0.13.0/thrift-0.13.0.tar.gz" \
  -O thrift-0.13.0.tar.gz
tar -xvf ./thrift-0.13.0.tar.gz
cd thrift-0.13.0

./configure --prefix=<thrift_install_dir> --silent --without-python \
  --enable-libtool-lock --enable-tutorial=no --enable-tests=no      \
  --with-libevent --with-zlib --without-nodejs --without-lua        \
  --without-ruby --without-csharp --without-erlang --without-perl   \
  --without-php --without-php_extension --without-dart              \
  --without-haskell --without-go --without-rs --without-haxe        \
  --without-dotnetcore --without-d --without-qt4 --without-qt5      \
  --without-java

make install -j $(nproc)
```

### Compile GTest/Googletest from source
The `libgtest-dev` package contains only the source files of GTest, but the
binaries are missing. You have to compile GTest manually.

```bash
mkdir gtest
cp -R /usr/src/googletest/* ./gtest

cd gtest
mkdir build
cd build

cmake .. -DCMAKE_INSTALL_PREFIX=<gtest_install_dir>
make install -j $(nproc)
```

### Install Node.js
Newer version of Node.js is required for the project, `nodejs` >= 18.

```bash
curl -sL https://deb.nodesource.com/setup_18.x -o nodesource_setup.sh
sudo bash nodesource_setup.sh
sudo apt install nodejs
```

## Build CodeCompass
Follow instructions on https://github.com/wbqpk3/CodeCompass/blob/master/doc/deps.md#build-codecompass <br>
and use CMake flag:
```bash
-DCMAKE_CXX_COMPILER=g++-9
```
