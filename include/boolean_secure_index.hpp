#include <memory>
#include <string_view>
#include <string>

using std::shared_ptr;
using std::make_shared;
using std::string

namespace alex::encrypted_search
{
    // Suppose the secret is s. Then, there is some cryptographic hash
    // function h : string -> unsigned int in which h(s) = H.
    // 
    // Parameter L tags the 'level' of the hash, i.e., we only provide
    // operations like and(cipher<bool>...)
    //
    // this permits a form of strong typing
    template <typename T, unsigned int H, unsigned int L>
    struct noisy_cipher
    {
        // code_t : first 4 bytes is a tag for the value type.
        // so, when constructing, say, a cipher<T,...> from a serialization
        // make sure the value type matches the value type for T.
        
        //is the value type
        using code_t = string;
        noisy_cipher(code_t code) : code(code)
        {

        }

        // the noisy cipher maps set these values
        double fpr, fnr;

        // this is some noisy cipher encoding of a value of type T.
        code_t code;
    };


    struct noisy_cipher
    {
        // code_t : first 4 bytes is a tag for the value type.
        // so, when constructing, say, a cipher<T,...> from a serialization
        // make sure the value type matches the value type for T.
        
        //is the value type
        using code_t = string;
        noisy_cipher(code_t code) : code(code)
        {

        }

        // the noisy cipher maps set these values
        double fpr, fnr;

        // this is some noisy cipher encoding of a value of type T.
        code_t code;
    };


    template <typename T, unsigned int H, unsigned int L>
    struct cipher
    {
        // this is some noisy cipher encoding of a value of type T.
        string value;
    };


    // represents a cipher of a Boolean index of type T, i.e.,
    //     contains : (boolean_cipher_index<H,L,T>, cipher<T>) -> noisy_cipher<bool,H,L>
    //     id : boolean_cipher_index<H,L,T> -> cipher<string,H,L>

    template <unsigned int H, typename T>
    class boolean_cipher_index
    {
    public:
        const auto secret_hash = H;
        const auto level = 1;
        using value_type = cipher<T>;
        using return_type = noisy_cipher<bool,H,1>;

        auto id() const
        {
            return s_->id();
        }

        noisy_cipher<bool,H,1> operator()(cipher<T> const & x) const
        {
            return contains(x);
        }

        template <typename S>
        boolean_cipher_index(S s) : s(make_shared<model<S>>(s)) {};

    private:
        struct concept
        {
            virtual nosiy_cipher<bool,H,1> contains(cipher<T> const &) const = 0;
            virtual cipher<string> id() const = 0;
        };

        template <typename BooleanCipherIndex>
        struct model final : concept
        {
            model(BooleanSecureIndex const & s) : s_(s)
            {
                static_assert(typename BooleanCipherIndex::level == L &&
                              typename BooleanCipherIndex::secret_hash == H);
            }

            noisy_cipher<bool,H,1> contains(cipher<T> x) const override { return contains(s_, x); }

            cipher<string,H,1> id() const override { return id(s_); }

            BooleanSecureIndex s_;
        };

        shared_ptr<concept const &> s;
    };

    template <unsigned int H, unsigned int L, typename T>
    noisy_cipher<bool,H,1> contains(
        boolean_cipher_index<H,1,T> const & s,
        cipher<T,H,1> const & x)
    {
        return s.contains(x);
    }

    template <unsigned int H>
    cipher<string,H,1> id(boolean_cipher_index<H,1,T> const & s)
    {
        return s.id(x);
    }

    template <typename H, typename L>
    noisy_cipher<bool,H,L+1> logical_or(
        noisy_cipher<bool,H,L> const & x,
        noisy_cipher<bool,H,L> const & y)
    {

        // construct a noisy_cipher_or<H,L> from a serialization.
        // note that we may also assume that we have a program that
        // can deal with it for scripting, but maybe nice to have it
        // available in a strong-typing programmatic c++ form also.
        //
        // 
    }
}