
template <class Stream> Stream& operator >> (Stream& stream, landscape4::BSPTree& tree)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> tree.nodes;
	}

	return stream;
}

template <class Stream> Stream& operator >> (Stream& stream, landscape4::BSPTree2& tree)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> tree.nodes;
		stream >> tree.polygons;
	}

	return stream;
}
		
