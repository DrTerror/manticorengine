#include <unordered_map>
#include <vector>
#include <bitset>
#include <memory>

constexpr std::size_t MAX_COMPONENTS = 64;

using EntityID = std::size_t;

class Entity {
public:
    EntityID id;
    explicit Entity(EntityID id) : id(id) {}

};
