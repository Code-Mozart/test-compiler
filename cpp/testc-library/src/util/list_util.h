#pragma once

#include "basic/include.h"

namespace testc {

    // helper function to move all elements from list to another
    // #parameter source:
    //   the list from which the elements will be moved
    // #parameter destination:
    //   the list to which the elements from the source will be added
    // #returns:
    //   the destination list.
    template<typename T>
    inline List<T>& move_elements(const List<T>& source, List<T>& destination) {
        for (const T& t : source) {
            destination.emplace_back(std::move(t));
        }
        return destination;
    }

    template<typename T>
    inline void destroy_elements(List<T>& list) {
        for (T& t : list) {
            destroy(t);
        }
    }

}
