
#include "elevation.hpp"

namespace Elevation {
std::vector<LayerConnection> allLayerConnections;

float getConnectionCost(ConnectionType type) {
  switch (type) {
  case CONNECTION_JUMP_DOWN:
    return 1.0f;
  case CONNECTION_LADDER:
    return 2.0f;
  case CONNECTION_GRAPPLE:
    return 3.0f;
  default:
    return 2.0f;
  }
}

void addConnection(int x, int y, int fromLayer, int toLayer,
                   ConnectionType type) {
  LayerConnection connection;
  connection.toLayer = toLayer;
  connection.fromLayer = fromLayer;
  connection.x = x;
  connection.y = y;
  connection.type = type;
  connection.movementCost = getConnectionCost(type);

  connection.biDirectional = (type != CONNECTION_JUMP_DOWN);

  allLayerConnections.push_back(connection);

  // biDirectional conncetions need an opposite, reverse connection to go along
  // side it(so you can climb down a ladder as well as up one)
  if (connection.biDirectional) {
    LayerConnection reverse = connection;
    reverse.fromLayer = toLayer;
    reverse.toLayer = fromLayer;
    allLayerConnections.push_back(reverse);
  }
}

void clearConnections() { allLayerConnections.clear(); }

bool canTraverseConnection(const LayerConnection &connection,
                           int currentLayer) {
  // check if a unit's currentLayer is = to the fromLayer of the connection
  return (connection.fromLayer == currentLayer);
}
std::vector<LayerConnection> getAllConnectionsAt(int x, int y,
                                                 int currentLayer) {
  std::vector<LayerConnection> connectionsAt;
  for (const auto &conn : allLayerConnections) {
    if (conn.x == x && conn.y == y && conn.fromLayer == currentLayer) {
      connectionsAt.push_back(conn);
    }
  }
  return connectionsAt;
}

} // namespace Elevation
