// Copyright (c) 1997 James Clark
// See the file COPYING for copying permission.

#ifndef CharMap_DEF_INCLUDED
#define CharMap_DEF_INCLUDED 1

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

#ifdef SP_MULTI_BYTE

template<class T>
CharMap<T>::CharMap()
{
}

template<class T>
CharMap<T>::CharMap(T dflt)
{
  for (size_t i = 0; i < 256; i++)
    lo_[i] = dflt;
  for (size_t i = 0; i < blocks; i++)
    values_[i].value = dflt;
}

template<class T>
void CharMap<T>::setAll(T val)
{
  for (size_t i = 0; i < 256; i++)
    lo_[i] = val;
  for (size_t i = 0; i < blocks; i++) {
    values_[i].value = val;
    delete [] values_[i].values;
    values_[i].values = 0;
  }
}

template<class T>
void CharMap<T>::swap(CharMap<T> &map)
{
  for (size_t i = 0; i < 256; i++) {
    T tem(lo_[i]);
    lo_[i] = map.lo_[i];
    map.lo_[i] = tem;
  }
  for (size_t i = 0; i < blocks; i++)
    values_[i].swap(map.values_[i]);
}

template<class T>
void CharMap<T>::setChar(Char c, T val)
{
  if (c < 256) {
    lo_[c] = val;
    return;
  }
  CharMapBlock<T> &bk = values_[blockIndex(c)];
  if (bk.values) {
    CharMapPage<T> &pg = bk.values[pageIndex(c)];
    if (pg.values) {
      CharMapColumn<T> &column = pg.values[columnIndex(c)];
      if (column.values)
        column.values[cellIndex(c)] = val;
      else if (val != column.value) {
        column.values = new T[columnSize];
        for (size_t i = 0; i < columnSize; i++)
  	  column.values[i] = column.value;
        column.values[cellIndex(c)] = val;
      }
    }
    else if (val != pg.value) {
      pg.values = new CharMapColumn<T>[columnsPerPage];
      for (size_t i = 0; i < columnsPerPage; i++)
        pg.values[i].value = pg.value;
      CharMapColumn<T> &column = pg.values[columnIndex(c)];
      column.values = new T[cellsPerColumn];
      for (size_t i = 0; i < cellsPerColumn; i++)
        column.values[i] = column.value;
      column.values[cellIndex(c)] = val;
    }
  }
  else if (val != bk.value) {
    bk.values = new CharMapPage<T>[pagesPerBlock];
    for (size_t i = 0; i < pagesPerBlock; i++)
      bk.values[i].value = bk.value;
    CharMapPage<T> &page = bk.values[pageIndex(c)];
    page.values = new CharMapColumn<T>[columnsPerPage];
    for (size_t i = 0; i < columnsPerPage; i++)
      page.values[i].value = page.value;
    CharMapColumn<T> &column = page.values[columnIndex(c)];
    column.values = new T[cellsPerColumn];
    for (size_t i = 0; i < cellsPerColumn; i++)
      column.values[i] = column.value;
    column.values[cellIndex(c)] = val;
  }
} 

template<class T>
void CharMap<T>::setRange(Char from, Char to, T val)
{
  for (; from < 256; from++) { 
    lo_[from] = val;
    if (from == to)
      return;
  }
  do {
    if ((from & (columnSize - 1)) == 0
        && to - from >= columnSize - 1) {
      if ((from & (pageSize - 1)) == 0
	  && to - from >= pageSize - 1) {
        if ((from & (blockSize - 1)) == 0
	    && to - from >= blockSize - 1) {
	  // Set a complete block.
	  CharMapBlock<T> &bk = values_[blockIndex(from)];
          bk.value = val;
          delete [] bk.values;  
          bk.values = 0; 
	  from += blockSize - 1;
        }
        else {
	  // Set a complete page.
	  CharMapBlock<T> &bk = values_[blockIndex(from)];
          if (bk.values) {
	    CharMapPage<T> &pg = bk.values[pageIndex(from)];
	    pg.value = val;
	    delete [] pg.values;
	    pg.values = 0;
          }
          else if (val != bk.value) {
	    // split the block
	    bk.values = new CharMapPage<T>[pagesPerBlock];
            for (size_t i = 0; i < pagesPerBlock; i++)
	      bk.values[i].value = bk.value;
	    CharMapPage<T> &page = bk.values[pageIndex(from)];
            page.value = val;
	  }
	  from += pageSize - 1;
        }
      }
      else {
	// Set a complete column.
	CharMapBlock<T> &bk = values_[blockIndex(from)];
        if (bk.values) {
	  CharMapPage<T> &pg = bk.values[pageIndex(from)];
	  if (pg.values) {
	    CharMapColumn<T> &column = pg.values[columnIndex(from)];
	    column.value = val;
	    delete [] column.values;
	    column.values = 0;
	  }
	  else if (val != pg.value) {
	    // split the page
	    pg.values = new CharMapColumn<T>[columnsPerPage];
            for (size_t i = 0; i < columnsPerPage; i++)
	      pg.values[i].value = pg.value;
	    CharMapColumn<T> &column = pg.values[columnIndex(from)];
	    column.value = val;
	  }
        }
        else if (val != bk.value) {
	  // split the block
	  bk.values = new CharMapPage<T>[pagesPerBlock];
          for (size_t i = 0; i < pagesPerBlock; i++)
	    bk.values[i].value = bk.value;
	  CharMapPage<T> &pg = bk.values[pageIndex(from)];
          pg.value = val;
	  // split the page
	  pg.values = new CharMapColumn<T>[columnsPerPage];
          for (size_t i = 0; i < columnsPerPage; i++)
	    pg.values[i].value = pg.value;
	  CharMapColumn<T> &column = pg.values[columnIndex(from)];
	  column.value = val;
	}
	from += columnSize - 1;
      }
    }
    else
      setChar(from, val);
  } while (from++ != to);
}

template<class T>
CharMapBlock<T>::CharMapBlock()
: values(0)
{
}

template<class T>
CharMapBlock<T>::CharMapBlock(const CharMapBlock<T> &bk)
{
  if (bk.values) {
    values = new CharMapPage<T>[pagesPerBlock];
    for (size_t i = 0; i < pagesPerBlock; i++)
      values[i] = bk.values[i];
  }
  else {
    value = bk.value;
    values = 0;
  }
}

template<class T>
void CharMapBlock<T>::operator=(const CharMapBlock<T> &bk)
{
  if (bk.values) {
    if (!values)
      values = new CharMapPage<T>[pagesPerBlock];
    for (size_t i = 0; i < pagesPerBlock; i++)
      values[i] = bk.values[i];
  }
  else {
    if (values) {
      delete [] values;
      values = 0;
    }
    value = bk.value;
  }
}

template<class T>
CharMapBlock<T>::~CharMapBlock()
{
  delete [] values;
}

template<class T>
void CharMapBlock<T>::swap(CharMapBlock<T> &bk)
{
  {
    CharMapPage<T> *tem = values;
    values = bk.values;
    bk.values = tem;
  }
  {
    T tem(value);
    value = bk.value;
    bk.value = tem;
  }
}

template<class T>
CharMapPage<T>::CharMapPage()
: values(0)
{
}

template<class T>
CharMapPage<T>::CharMapPage(const CharMapPage<T> &pg)
{
  if (pg.values) {
    values = new CharMapColumn<T>[columnsPerPage];
    for (size_t i = 0; i < columnsPerPage; i++)
      values[i] = pg.values[i];
  }
  else {
    value = pg.value;
    values = 0;
  }
}

template<class T>
void CharMapPage<T>::operator=(const CharMapPage<T> &pg)
{
  if (pg.values) {
    if (!values)
      values = new CharMapColumn<T>[columnsPerPage];
    for (size_t i = 0; i < columnsPerPage; i++)
      values[i] = pg.values[i];
  }
  else {
    if (values) {
      delete [] values;
      values = 0;
    }
    value = pg.value;
  }
}

template<class T>
CharMapPage<T>::~CharMapPage()
{
  delete [] values;
}

template<class T>
void CharMapPage<T>::swap(CharMapPage<T> &pg)
{
  {
    CharMapColumn<T> *tem = values;
    values = pg.values;
    pg.values = tem;
  }
  {
    T tem(value);
    value = pg.value;
    pg.value = tem;
  }
}

template<class T>
CharMapColumn<T>::CharMapColumn()
: values(0)
{
}

template<class T>
CharMapColumn<T>::CharMapColumn(const CharMapColumn<T> &col)
{
  if (col.values) {
    values = new T[cellsPerColumn];
    for (size_t i = 0; i < cellsPerColumn; i++)
      values[i] = col.values[i];
  }
  else {
    values = 0;
    value = col.value;
  }
}

template<class T>
void CharMapColumn<T>::operator=(const CharMapColumn<T> &col)
{
  if (col.values) {
    if (!values)
      values = new T[cellsPerColumn];
    for (size_t i = 0; i < cellsPerColumn; i++)
      values[i] = col.values[i];
  }
  else {
    if (values) {
      delete [] values;
      values = 0;
    }
    value = col.value;
  }
}

template<class T>
CharMapColumn<T>::~CharMapColumn()
{
  delete [] values;
}

#else /* not SP_MULTI_BYTE */

template<class T>
CharMap<T>::CharMap()
{
}

template<class T>
CharMap<T>::CharMap(T dflt)
{
  for (int i = 0; i < 256; i++)
    values_[i] = dflt;
}

template<class T>
void CharMap<T>::setAll(T val)
{
  for (size_t i = 0; i < 256; i++)
    values_[i] = val;
}

template<class T>
void CharMap<T>::setRange(Char from, Char to, T val)
{
  do {
    values_[from] = val;
  } while (from++ != to);
}

template<class T>
void CharMap<T>::swap(CharMap<T> &map)
{
  for (size_t i = 0; i < 256; i++) {
    T tem(values_[i]);
    values_[i] = map.values_[i];
    map.values_[i] = tem;
  }
}

#endif /* not SP_MULTI_BYTE */

#ifdef SP_NAMESPACE
}
#endif

#endif /* not CharMap_DEF_INCLUDED */
