template<typename T>
T ListUtils::getByIndex(std::list<T> l, short i) {
  // TODO maybe we should use the same exc-like mechanism as in ds18b20-sensor.ino.
  if (i >= l.size()) return nullptr;
  auto it = l.begin();
  std::advance(it, i);
  return *it;
}
