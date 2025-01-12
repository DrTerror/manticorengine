struct Component {
    virtual ~Component() = default;
};

using ComponentType = std::size_t;

inline ComponentType getUniqueComponentTypeID() {
    static ComponentType lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentType getComponentTypeID() noexcept {
    static ComponentType typeID = getUniqueComponentTypeID();
    return typeID
      ;
}

class EntityManager {
private:
    std::vector<std::unique_ptr<Component>> components[MAX_COMPONENTS];
    std::unordered_map<EntityID, std::bitset<MAX_COMPONENTS>> entityComponentMask;

public:
    Entity createEntity() {
        static EntityID lastID = 0;
        return Entity(lastID++);
    }

    template <typename T, typename... Args>
    void addComponent(EntityID entity, Args&&... args) {
        ComponentType type = getComponentTypeID<T>();
        if (type >= MAX_COMPONENTS) throw std::runtime_error("Exceeded max components!");
        components[type].emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        entityComponentMask[entity].set(type);
    }

    template <typename T>
    T* getComponent(EntityID entity) {
        ComponentType type = getComponentTypeID<T>();
        if (entityComponentMask[entity].test(type)) {
            return static_cast<T*>(components[type][entity].get());
        }
        return nullptr;
    }

    std::bitset<MAX_COMPONENTS> getEntityMask(EntityID entity) {
        return entityComponentMask[entity];
    }
};

class System {
public:
    virtual void update(EntityManager& entityManager, float deltaTime) = 0;
};

