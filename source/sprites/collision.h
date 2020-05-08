// Test whether or not a given tile should stop player movement. 0: no collision, 1: collision
// This function has side effects for special tiles, but those side effects are ignored for non-players.
unsigned char test_collision(unsigned char tileId, unsigned char isPlayer);