#pragma once

#include <sstream>

//HACK to gain access to private members in directed_graph
//Otherwise the directed graph cannot be serialized currently
#define private public
#include <boost/graph/directed_graph.hpp>
#undef private

#include <boost/graph/adj_list_serialize.hpp>
#include <boost/uuid/uuid.hpp>

#include <boost/multi_array.hpp>

#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/array.hpp>

#include <boost/container/container_fwd.hpp>
#include <boost_serialization/DynamicSizeSerialization.hpp>
#include <boost/serialization/utility.hpp>

namespace boost { namespace serialization
{

    /** Serializes boost::uuids::uuid */
    template<typename _Archive>
    inline void serialize(
        _Archive & ar,
        boost::uuids::uuid& uuid,
        const unsigned int version)
    {
        ar & boost::serialization::make_nvp("uuid", boost::serialization::make_binary_object(uuid.data, uuid.size()));
    }

    /** Serializes boost::directed_graph */
    template<class Archive, typename VertexProp, typename EdgeProp, typename GraphProp>
    inline void serialize(Archive & ar,
                   boost::directed_graph<VertexProp, EdgeProp, GraphProp>& graph,
                   const unsigned int version)
    {
        ar & boost::serialization::make_nvp("adjacency_list", graph.impl());
        ar & boost::serialization::make_nvp("m_num_vertices", graph.m_num_vertices);
        ar & boost::serialization::make_nvp("m_num_edges", graph.m_num_edges);
        ar & boost::serialization::make_nvp("m_max_vertex_index", graph.m_max_vertex_index);
        ar & boost::serialization::make_nvp("m_max_edge_index", graph.m_max_edge_index);
    }

    /** Serializes boost::multi_array */

    template<class Archive, typename ValueType>
    void load( Archive & ar,
               boost::multi_array<ValueType, 2> & t,
               const unsigned int version )
    {
        typedef boost::multi_array<ValueType, 2> multi_array_;
        typedef typename multi_array_::size_type size_;
        size_ n0;
        ar >> BOOST_SERIALIZATION_NVP( n0 );
        size_ n1;
        ar >> BOOST_SERIALIZATION_NVP( n1 );
        t.resize( boost::extents[n0][n1] );
        ar >> make_array( t.data(), t.num_elements() );
    }

    template<typename Archive, typename ValueType>
    void save( Archive & ar,
               const boost::multi_array<ValueType, 2> & t,
               const unsigned int version )
    {
        typedef boost::multi_array<ValueType, 2> multi_array_;
        typedef typename multi_array_::size_type size_;
        size_ n0 = ( t.shape()[0] );
        ar << BOOST_SERIALIZATION_NVP( n0 );
        size_ n1 = ( t.shape()[1] );
        ar << BOOST_SERIALIZATION_NVP( n1 );
        ar << boost::serialization::make_array( t.data(), t.num_elements() );
    }

    template<class Archive, typename ValueType>
    void serialize( Archive & ar,
                    boost::multi_array<ValueType, 2>& t,
                    const unsigned int version )
    {
        split_free( ar, t, version );
    }    



#define ROCK_BOOST_SERIALIZATION_SET_OR_MAP(name, classes, parameters) \
    template<class Archive, classes> \
    inline void serialize(Archive& ar, name<parameters>& t, const unsigned int version) { split_free(ar, t, version); } \
    template<class Archive, classes> \
    void save(Archive & ar, const name<parameters>& t, const unsigned int /* version */ ) { \
        uint64_t size(t.size()); saveSizeValue(ar, size); \
        for(typename name<parameters>::const_iterator it = t.begin(); it != t.end(); ++it) { ar << *it; } \
    } \
    template<class Archive, classes> \
    void load(Archive & ar, name<parameters>& t, const unsigned int /* version */ ) { \
        uint64_t count; loadSizeValue(ar, count); \
        t.clear(); t.reserve(count); \
        for(size_t i=0; i<count; ++i) { \
            typename name<parameters>::value_type object_; \
            ar >> object_; \
            t.insert(t.end(), std::move(object_)); \
        } \
    } \

#define ROCK_BOOST_SERIALIZATION_SET_CLASSES class Key, class Compare, class Allocator
#define ROCK_BOOST_SERIALIZATION_SET_PARAMETERS Key, Compare, Allocator
    ROCK_BOOST_SERIALIZATION_SET_OR_MAP(boost::container::flat_set,      ROCK_BOOST_SERIALIZATION_SET_CLASSES, ROCK_BOOST_SERIALIZATION_SET_PARAMETERS)
    ROCK_BOOST_SERIALIZATION_SET_OR_MAP(boost::container::flat_multiset, ROCK_BOOST_SERIALIZATION_SET_CLASSES, ROCK_BOOST_SERIALIZATION_SET_PARAMETERS)
#undef ROCK_BOOST_SERIALIZATION_SET_CLASSES
#undef ROCK_BOOST_SERIALIZATION_SET_PARAMETERS
#define ROCK_BOOST_SERIALIZATION_MAP_CLASSES class Key, class Value, class Compare, class Allocator
#define ROCK_BOOST_SERIALIZATION_MAP_PARAMETERS Key, Value, Compare, Allocator
    ROCK_BOOST_SERIALIZATION_SET_OR_MAP(boost::container::flat_map,      ROCK_BOOST_SERIALIZATION_MAP_CLASSES, ROCK_BOOST_SERIALIZATION_MAP_PARAMETERS)
    ROCK_BOOST_SERIALIZATION_SET_OR_MAP(boost::container::flat_multimap, ROCK_BOOST_SERIALIZATION_MAP_CLASSES, ROCK_BOOST_SERIALIZATION_MAP_PARAMETERS)
#undef ROCK_BOOST_SERIALIZATION_MAP_CLASSES
#undef ROCK_BOOST_SERIALIZATION_MAP_PARAMETERS

#undef ROCK_BOOST_SERIALIZATION_SET_OR_MAP

}}
