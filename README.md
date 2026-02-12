# Serum2 VST3 Renderer - Milestone A Test

## Quick Start

This is a minimal test for **Milestone A** - plugin loading and basic rendering.

### Prerequisites

1. **JUCE Framework** (will be downloaded as submodule)
2. **Serum2 VST3** installed in: `C:\Program Files\Common Files\VST3\`
3. **CMake** 3.15 or later
4. **Visual Studio 2019** or later (on Windows)

### Build Instructions

```bash
# 1. Clone/navigate to project
cd serum_project/render_project

# 2. Initialize JUCE submodule
git submodule init
git submodule update

# 3. Create build directory
mkdir build
cd build

# 4. Generate project files
cmake ..

# 5. Build
cmake --build . --config Release

# 6. Run Milestone A test
Release\BatchRenderer.exe
```

### Expected Output

If successful, the test will:
1. Scan for VST3 plugins (cached after first run)
2. Find and load Serum2
3. Generate synthetic MIDI (C4 note, velocity 100)
4. Render 2 seconds + 1 second tail to WAV
5. Save to: `data/outwav/milestone_a_test.wav`
6. Print audio statistics (peak, RMS)

### Verification

**Listen to the WAV file** - you should hear a tone from Serum2.

If you hear silence:
- Serum2 may need preset initialization (Milestone B will handle this)
- Check that Serum2 is properly installed
- Verify the console output for errors

### Troubleshooting

**"Serum2 not found"**
- Ensure Serum2 VST3 is installed
- Check the exact path: `C:\Program Files\Common Files\VST3\`
- The scanner looks for plugin names containing "Serum" (case-insensitive)

**Build errors**
- Ensure JUCE submodule is initialized: `git submodule update --init --recursive`
- Check CMake version: `cmake --version` (need 3.15+)
- Try cleaning build directory and rebuilding

**Link errors**
- JUCE modules may need dependencies (Windows Multimedia, etc.)
- These should be handled automatically by JUCE's CMake integration

### Next Steps

After validating Milestone A:
1. **Milestone B** - Implement StateCapturer GUI for preset capture/restore
2. **Milestone C** - Implement MIDI file loading
3. **Milestone D** - Implement batch dataset generation

## Project Structure

```
src/
  common/     - Utilities (Hash, Paths, Log)
  vst/        - Plugin management (Scanner, Factory, State IO)
  midi/       - MIDI generation (SyntheticMidiGenerator)
  render/     - Streaming renderer (OfflineRenderer, WavWriter, AudioStats)
  apps/       - Applications (BatchRenderer test, StateCapturer placeholder)
```

## Architecture Notes

- **Streaming rendering**: Memory usage constant regardless of render length
- **Plugin caching**: First scan creates `plugin_cache.xml`, subsequent runs load from cache
- **SHA256**: All hashing uses SHA256 (not SHA1)
- **Synchronous loading**: Deterministic plugin instantiation
