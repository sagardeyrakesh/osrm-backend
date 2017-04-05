#ifndef OSRM_EXTRACTOR_IO_HPP
#define OSRM_EXTRACTOR_IO_HPP

#include "extractor/datasources.hpp"
#include "extractor/nbg_to_ebg.hpp"
#include "extractor/segment_data_container.hpp"

#include "storage/io.hpp"

#include <boost/assert.hpp>

namespace osrm
{
namespace extractor
{
namespace serialization
{

inline void read(storage::io::FileReader &reader, std::vector<NBGToEBG> &mapping)
{
    reader.DeserializeVector(mapping);
}

inline void write(storage::io::FileWriter &writer, const std::vector<NBGToEBG> &mapping)
{
    writer.SerializeVector(mapping);
}

inline void read(storage::io::FileReader &reader, Datasources &sources)
{
    reader.ReadInto(sources);
}

inline void write(storage::io::FileWriter &writer, Datasources &sources)
{
    writer.WriteFrom(sources);
}

template <>
inline void read(storage::io::FileReader &reader, SegmentDataContainer &segment_data)
{
    auto num_indices = reader.ReadElementCount32();
    segment_data.index.resize(num_indices);
    reader.ReadInto(segment_data.index.data(), num_indices);

    auto num_entries = reader.ReadElementCount32();
    segment_data.nodes.resize(num_entries);
    segment_data.fwd_weights.resize(num_entries);
    segment_data.rev_weights.resize(num_entries);
    segment_data.fwd_durations.resize(num_entries);
    segment_data.rev_durations.resize(num_entries);
    segment_data.datasources.resize(num_entries);

    reader.ReadInto(segment_data.nodes);
    reader.ReadInto(segment_data.fwd_weights);
    reader.ReadInto(segment_data.rev_weights);
    reader.ReadInto(segment_data.fwd_durations);
    reader.ReadInto(segment_data.rev_durations);
    reader.ReadInto(segment_data.datasources);
}

template <>
inline void write(storage::io::FileWriter &writer, const SegmentDataContainer &segment_data)
{
    writer.WriteElementCount32(segment_data.index.size());
    writer.WriteFrom(segment_data.index);

    writer.WriteElementCount32(segment_data.nodes.size());
    BOOST_ASSERT(segment_data.fwd_weights.size() == segment_data.nodes.size());
    BOOST_ASSERT(segment_data.rev_weights.size() == segment_data.nodes.size());
    BOOST_ASSERT(segment_data.fwd_durations.size() == segment_data.nodes.size());
    BOOST_ASSERT(segment_data.rev_durations.size() == segment_data.nodes.size());
    BOOST_ASSERT(segment_data.datasources.size() == segment_data.nodes.size());
    writer.WriteFrom(segment_data.nodes);
    writer.WriteFrom(segment_data.fwd_weights);
    writer.WriteFrom(segment_data.rev_weights);
    writer.WriteFrom(segment_data.fwd_durations);
    writer.WriteFrom(segment_data.rev_durations);
    writer.WriteFrom(segment_data.datasources);
}
}
}
}

#endif
