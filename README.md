# Build: Compile & Upload
- File Structure:
```
path/to/sketch/folder/SketchName/SketchName.ino
```
- Compile
```
cd path/to/sketch/folder/
arduino-cli compile -b arduino:avr:uno SketchName
```

- Upload
```
cd path/to/sketch/folder/
arduino-cli upload -b arduino:avr:uno SketchName
```

- Install library
```sh
arduino-cli lib install LibraryName
# example
arduino-cli lib install LiquidCrystal
```