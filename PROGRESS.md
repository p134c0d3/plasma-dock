# Plasma Dock - Development Progress

## Completed: Phase 1 (Foundation & Multi-Monitor Support)

### Core Infrastructure
- [x] TOML configuration parser (`TomlParser`)
- [x] Complete `Config` class with all settings
- [x] `Monitor` wrapper class for screen information
- [x] `DockWindow` with QML content loading

### Multi-Monitor Support
- [x] Configurable display modes: `all`, `primary`, `specific`
- [x] Per-monitor dock positioning
- [x] Dynamic screen hotplug (add/remove monitors)

### UI Components
- [x] `main.qml` - Root container with styling
- [x] `DockPanel.qml` - App layout with separator and trash
- [x] `IconButton.qml` - Interactive icons with tooltips
- [x] `Tooltip.qml` - Custom tooltip component

### Build System
- [x] CMake configuration with Qt6/KF6
- [x] QML resource file (qml.qrc)
- [x] Command-line timeout option (`-t/--timeout`)

---

## Completed: Phase 2 (Wayland Layer-Shell Integration)

### Critical Window Behavior
- [x] Implement Wayland layer-shell protocol for proper dock behavior
- [x] Configure compositor to avoid covering dock area (exclusive zone)
- [x] Set proper anchors (bottom edge by default)
- [x] Handle X11 fallback for non-Wayland sessions
- [ ] Implement background blur using `KWindowEffects` (optional enhancement)
- [x] Set correct window flags for Plasma compatibility

### Implementation Details
- **Wayland Support**: Using `LayerShellQt::Window` API for proper Wayland protocol integration
  - Sets layer to `LayerTop` (positions above normal windows)
  - Anchors to bottom + left + right edges
  - Exclusive zone set to dock height (reserves screen space)
  - Keyboard interactivity set to `OnDemand`
  - Scope identifier set to "plasma-dock"
  
- **X11 Fallback**: Graceful fallback using Qt window flags for non-Wayland sessions
  
- **Runtime Detection**: Detects session type via `XDG_SESSION_TYPE` environment variable
  - Wayland: Uses layer-shell protocol
  - X11/Other: Uses Qt window flags fallback

### Modified Files
- `CMakeLists.txt` - Added LayerShellQt dependency
- `src/CMakeLists.txt` - Linked LayerShellQt::Interface
- `src/main.cpp` - Added LayerShellQt::Shell::useLayerShell() initialization
- `src/dockwindow.h` - Added setupX11Fallback() method
- `src/dockwindow.cpp` - Implemented setupWayland() and setupX11Fallback() with runtime detection

### Testing Results
- ✅ Clean build with no errors
- ✅ Executable runs on Wayland session without errors
- ✅ Dock window properly configured as layer-shell panel
- ✅ X11 fallback path available

---

## Next: Phase 3 (Application Management)

### Application Tracking & Pinning
- [ ] Track running applications per screen
- [ ] Load desktop files for pinned apps
- [ ] Implement running indicator logic
- [ ] Pinned apps persistence via config file

---

## Future Phases (Planned)

### Phase 4: Visibility Modes
- [ ] Intelli-hide (detect maximized/fullscreen windows)
- [ ] Auto-hide with configurable delay
- [ ] Slide animations

### Phase 5: UI Polish & Trash
- [ ] Drag-and-drop support
- [ ] Application launching
- [ ] Trash bin click handler (`trash:/` URL)
- [ ] Visual polish (animations, transitions)

### Phase 6: Edge Cases & Polish
- [ ] Support for left/right dock positioning
- [ ] HiDPI scaling
- [ ] Theme integration
- [ ] Performance optimization

---

## Building & Testing

```bash
# Full build
cmake -B build && cmake --build build

# Test with 5-second timeout
./build/src/plasma-dock -t 5

# Run without timeout (Ctrl+C to exit)
./build/src/plasma-dock
```

## Configuration File

Copy the template to your config location:
```bash
mkdir -p ~/.config/plasma-dock
cp assets/config_template.toml ~/.config/plasma-dock/config.toml
```

Edit `~/.config/plasma-dock/config.toml` to customize:
- Monitor mode (all/primary/specific)
- Visibility mode (always/auto-hide/intelli-hide)
- Appearance (color, opacity, blur, borders, radius)
- Dock dimensions and margins
