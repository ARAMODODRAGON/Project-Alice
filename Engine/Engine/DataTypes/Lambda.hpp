#ifndef _DATATYPES_LAMBDA_HPP
#define _DATATYPES_LAMBDA_HPP

namespace detail {

	template<class RETURN, class... PARAMS>
	struct LMWrapper {
		virtual RETURN Invoke(PARAMS... params) = 0;
		virtual LMWrapper* Copy() const = 0;
	};

	template<class L, class RETURN, class... PARAMS>
	struct LMWrapperWithType : public LMWrapper<RETURN, PARAMS...> {
		L lambda;

		LMWrapperWithType(L lambda_) : lambda(lambda_) { }

		virtual RETURN Invoke(PARAMS... params) override {
			return lambda(params...);
		}

		virtual LMWrapper<RETURN, PARAMS...>* Copy() const override {
			return new LMWrapperWithType<L, RETURN, PARAMS...>(lambda);
		}
	};

}

template<class RETURN, class... PARAMS>
class Lambda {

	detail::LMWrapper<RETURN, PARAMS...>* wrapper;

public:

	Lambda() : wrapper(nullptr) { }

	template<class L>
	Lambda(L lambda) : wrapper(nullptr) {
		wrapper = new detail::LMWrapperWithType<L, RETURN, PARAMS...>(lambda);
	}

	template<class L>
	Lambda& operator=(L lambda) {
		if (wrapper) delete wrapper;
		wrapper = new detail::LMWrapperWithType<L, RETURN, PARAMS...>(lambda);
		return *this;
	}

	Lambda(const Lambda& other) : wrapper(nullptr) { 
		if (other.wrapper) wrapper = other.wrapper->Copy();
	}
	Lambda& operator=(const Lambda& other) {
		if (wrapper) delete wrapper;
		wrapper = nullptr;
		if (other.wrapper) wrapper = other.wrapper->Copy();
		return *this;
	}
	Lambda(Lambda&& other) : wrapper(other.wrapper) {
		other.wrapper = nullptr;
	}
	Lambda& operator=(Lambda&& other) {
		if (wrapper) delete wrapper;
		wrapper = other.wrapper;
		other.wrapper = nullptr;
		return *this;
	}

	RETURN operator()(PARAMS... params) {
		return wrapper->Invoke(params...);
	}

};

#endif // !_DATATYPES_LAMBDA_HPP