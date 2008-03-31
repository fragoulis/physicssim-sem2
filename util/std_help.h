#pragma once

// *** STD helpers *** 
#define DELETE_VECTOR( type, vector )   \
    type::iterator i = vector.begin();  \
    for(; i != vector.end(); ++i )      \
    {                                   \
        type *obj = *i;                 \
        delete obj;                     \
        obj = 0;                        \
    }
    // end delete_vector

#define DELETE_MAP( type, map )     \
    type::iterator i = map.begin(); \
    for(; i != map.end(); ++i )     \
    {                               \
        delete i->second;           \
        i->second = 0;              \
    }
    // end delete_map

#define MAP_INSERT( map, key, value ) \
    map.insert( std::make_pair( key, value ) )