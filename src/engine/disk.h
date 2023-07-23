#pragma once

// load resources from an archive instead of disk
void loadPack (const char* filepath);

// load resources directly from disk,
// deallocate current datapack if loaded
void unloadPack ();