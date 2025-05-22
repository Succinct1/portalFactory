# portalFactory

## Clone the repository (with submodules)
```bash
git clone --recurse-submodules https://github.com/Succinct1/portalFactory.git
cd portalFactory
```

## Build Box2D and portalFactory

```bash
# Build Box2D
cd box2d
chmod +x build.sh
./build.sh
cd ..

# Build the project
mkdir build
cmake -S . -B build
cmake --build build
```

## Clean all generated files

```bash
chmod +x clean.sh
./clean.sh
```