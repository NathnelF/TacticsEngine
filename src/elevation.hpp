
#pragma once
#include <raylib.h>
#include <vector>

// this file will contain all the code that deals with elevation.
enum ElevationType {
  ELEVATION_GROUND = 0,
  ELEVATION_MID = 1,
  ELEBATION_HIGH = 2
};

enum ConnectionType {
  CONNECTION_JUMP_DOWN = 0,
  CONNECTION_LADDER = 1,
  CONNECTION_GRAPPLE = 2
};

struct LayerConnection {
  int x, y;
  int fromLayer, toLayer;
  ConnectionType type;
  float movementCost;
  bool biDirectional;
};

namespace Elevation {
  extern std::vector<LayerConnection> allLayerConnections;
  void addConnection(int x, int y, int fromLayer, int toLayer, ConnectionType type);
  void clearConnections();
  float getConnectionCost(ConnectionType type);
  bool canTraverseConnection(const LayerConnection& connection, int currentLayer);
  std::vector<LayerConnection> getAllConnectionsAt(int x, int y, int currentLayer);
}
