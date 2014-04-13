Crystal AMP (cAmp)
------------------

Audio Player with custom playlist and filtering.
Works on Windows and uses DirectX and bass.

Screenshots
-----------

![](https://raw.githubusercontent.com/cryham/cAmp/master/cAmp/screenshots.png)

Features
--------

new in version 1.04 (16 July 2010)

- font sizes (playlist and others)
- whole view loading/saving (window postion, size, visualization etc.)
- ice print visualization type 

version 1.02 (1 March 2010)

- supported audio formats: WAV,FLAC,OGG,MP3,MP2,MPC,APE,WV,WMA
- global Hot Keys via Windows Hook
- large Visualization (fft/oscilloscope) using Pixel Shader 2.0 

- song Rating (-3..+5, can be saved in filename), visible as symbol and background in one line
- rating Filtering in playlist (hides songs outside range)
- directories grouping (3 display modes), directory hiding/showing

- playlist Tabs, changeable number of columns and rows
- song Bookmarks, 3 levels (cyan, orange, red), keys to navigate
- song name Searching (on all tabs) 

- playlist slider displaying: visible range, cursor pos, playing pos, bookmarks, find results, and rating preview in whole playlist
- faster playlist managing through many shortcuts, operations like drop and insert are made normally at cursor, with shift at top, with ctrl at end of playlist
- extended mouse areas (song seek, prev/next, playlist slider)

- time coloring depending on song time, useless zeros not displayed
- custom colored fonts (created in CrystalFont)

Features that are NOT (and won't be) implemented

- Tiltle/Artist reading from file Tags - the song name displayed is the filename (if it's messy, rename)
- common Playlist files loading (m3u,pls,plc,cue, etc.) - using custom, extended, fast and own format .cp
- Skin support - all textures and some colors can be changed, the rest is in source code.
