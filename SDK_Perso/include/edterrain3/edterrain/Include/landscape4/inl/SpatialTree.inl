/**
 * Сериализация дерева. Через метод, чтоб не тащить все в паблик для <<
 */
template <typename T> template <typename S> void landscape4::SpatialTree<T>::serialize(S& stream)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> objects;
		stream >> boxes;
		stream >> nodes;
	}
}

template <typename S, typename T> S& operator >> (S& stream, landscape4::SpatialTree<T>& tree)
{
	tree.serialize(stream);
	return stream;
}
