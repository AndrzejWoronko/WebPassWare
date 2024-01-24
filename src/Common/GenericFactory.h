#ifndef GENERICFACTORY_H
#define GENERICFACTORY_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

template <typename ProductType, typename IdType = std::string, typename CreatorType = std::function<std::unique_ptr<ProductType>()>>
class GenericFactory
{
    std::unordered_map<IdType, CreatorType> creators_;

public:
    bool register_creator(IdType id, CreatorType creator)
    {
        bool is_inserted;
        std::tie(std::ignore, is_inserted) = creators_.insert(std::make_pair(std::move(id), std::move(creator)));

        return is_inserted;
    }

    std::unique_ptr<ProductType> create(const IdType& id) const
    {
        auto& creator = creators_.at(id);

        return creator();
    }
};

#endif // GENERICFACTORY_H
