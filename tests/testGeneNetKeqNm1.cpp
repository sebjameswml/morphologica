#include <morph/bn/Genome.h>
#include <morph/bn/GeneNet.h>

using std::endl;
using std::cout;

int main()
{
    // Note: compile time constants - i.e. not just const from early on in the program
    const size_t n = 5;
    const size_t k = 4;

    cout << "N=" << n << ", K=" << k << endl;
    morph::bn::Genome<n, k> g;
    g.randomize();
    morph::bn::GeneNet<n, k> gn;
    morph::bn::state_t state = 0x2;
    cout << "Gene net initial state:\n"
         << morph::bn::GeneNet<n,k>::state_table(state) << endl;
    // Develop according to g
    gn.develop (state, g);
    cout << "Gene net state is now:  " << morph::bn::GeneNet<n,k>::state_str(state) << endl;
    // Display the Genome table
    cout << g.table() << endl;

    return 0;
}
