#include <iostream>
#include <string>

template <class T>
struct kv {
    std::string key = "";
    int value_size = -1;
    T *value = new T;
    int *index = new int;
};

template <class T>
struct Dict {
  // attributes
  kv<T> *dictionary = new kv<T>;
  kv<T> empty;
  int size = 0;

  // methods
  T* get_value(std::string key) {
    for (int i = 0; i < this->size; i++) {
      if (this->dictionary[i].key == key) {
        return this->dictionary[i].value;
      }
    }
    return NULL;
  }

  int get_value_size(std::string key) {
    for (int i = 0; i < this->size; i++) {
      if (this->dictionary[i].key == key) {
        return this->dictionary[i].value_size;
      }
    }
    return -1;
  }

	kv<T> &find(std::string key) {
    for (int i = 0; i < this->size; i++) {
      if (this->dictionary[i].key == key) {
        return this->dictionary[i];
      }
    }

    return this->empty;
  }

  kv<T> get(std::string key) {
    for (int i = 0; i < this->size; i++) {
      if (this->dictionary[i].key == key) {
        return this->dictionary[i];
      }
    }

    return this->empty;
  }
  
  void append(kv<T> new_word) {
    *(this->dictionary + this->size) = new_word;
    std::cout << "Appended" << std::endl;
    this->size++;
  }

	void append(std::string key, int new_value_size, T *value, int *index) {
		kv<T> new_kv;
		new_kv.key = key;
		new_kv.value_size = new_value_size;
		new_kv.value = value;
    new_kv.index = index;

    *(this->dictionary + this->size) = new_kv;
    std::cout << "Appended" << std::endl;
    this->size++;
  }

  void update_key(std::string key, int new_value_size, T *value, int *index) {
    kv<T> &update_value = this->find(key);

    if (update_value.value_size != -1) {
      update_value.value = value;
      update_value.index = index;
		  update_value.value_size = new_value_size;

      std::cout << "Updated" << std::endl;
    } else {
      std::cout << "Error: Key not found." << std::endl;
    }
  }

	void delete_key(std::string key) {
    bool found_delete = false;

    for (int i = 0; i < this->size; i++) {
      if (this->dictionary[i].key == key)
        found_delete = true;

      if (found_delete && i < this->size - 1)
        *(this->dictionary + i) = *(this->dictionary + (i+1));   
    }

    if (found_delete) {
      std::cout << "Deleted" << std::endl;
      this->size--;
    }
    else
      std::cout << "Error: Key not found." << std::endl;
  }

	void print_value(std::string key) {
		kv<T> &need_kv = this->find(key);

    if (need_kv.value_size != -1) {
      std::cout << "key: " << need_kv.key << "\n" << "value: ";
      for (int i = 0; i < need_kv.value_size; i++) {
        if (i == need_kv.value_size - 1)
          std::cout << need_kv.value[i] << std::endl;
        else 
          std::cout << need_kv.value[i] << "\t";
      }
    } else {
      std::cout << "Error: Key not found." << std::endl;
    }
	}

	void print() {
		for (int i = 0; i < this->size; i++) {
			this->print_value(this->dictionary[i].key);
		}
	}
};