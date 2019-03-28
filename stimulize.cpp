#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define VERSION "v3.01"

// PROGRAM TO VECTORIZE IRSIM INPUTS

// REQUIRED FLAGS:
// --analyzer,-a <nodes...>       :   ENTER SPACE SEPARATED NODE NAMES
// --vector,-v <nodes...>         :   ENTER SPACE SEPARATED NODE NAMES TO VECTORIZE

// OPTIONAL FLAGS:
// --stepsize,-s <val>            :   SET STEPSIZE TO VAL ns, DEFAULT IS 5ns
// --test,-t <val_sequence...>    :   ENTER A TEST SEQUENCE OF VALUES

// EXAMPLE: 2 INPUT AND WITH (A,B) AS INPUTS AND (OUT) AS AN OUTPUT
// (A,B) WILL GO RESPECTIVELY FROM (0,0) -> (0,1) -> (1,0) -> (1,1)
// WITH A STEPSIZE OF 5ns.

// ./stimulize -analyzer A B OUT -v A B

// EXAMPLE: (IN_1,IN_2,IN_3) AS INPUTS AND (OUT_1,OUT_2) AS OUTPUTS
// (IN_1,IN_2,IN_3) WILL GO RESPECTIVELY FROM (0,0,0) -> (0,0,1) -> ... -> (1,1,0) -> (1,1,1)
// WITH A STEPSIZE OF 10ns

//./stimulize -a IN_1 IN_2 IN_3 OUT_A OUT_B -v IN_1 IN_2 IN_3 -s 10

//function to convert a binary number to grays number
//gray number only toggle 1 bit at a time so should give more stable output shapes
unsigned int binaryToGray( unsigned int bnum )
{
    return ( ( bnum ) ^ ( bnum >> 1 ) );
}

// FUNCTION TO PRINT THE STATE VECTOR
void printState( const std::vector<std::string>& inputs, int v_nodes, unsigned int number, std::string sequence )
{
    std::string binary = "";
    
    std::cout << "| VECTOR: [";
    
    v_nodes--;
    for( int i = 0; i < v_nodes; i++ )
    {
        std::cout << inputs[ i ] << ",";
    }
    
    std::cout << inputs[ v_nodes ] << "] = [";
    
    if( sequence == "" )
    {
        if( number & 1 )
        {
            binary = "1";
        }
        else
        {
            binary = "0";
        }
    
        number = number >> 1;
    
        v_nodes++;
        for( int i = 1; i < v_nodes; i++ )
        {
            if( number & 1 )
            {
                binary = "1," + binary;
            }
            else
            {
                binary = "0," + binary;
            }
            
            number = number >> 1;
        }
        
        std::cout << binary;
    }
    else
    {
        for( int i = 0; i < v_nodes; i++ )
        {
            std::cout << sequence[ i ] << ",";
        }
        
        std::cout << sequence[ v_nodes ];
    }
    
    std::cout << "]" << std::endl;
}

// FUNCTION TO GENERATE VECTORIZED INPUTS
void generateCommands( char** argv, int argc, int a_index, int a_nodes, int v_index, int v_nodes,
                      int t_index, int t_nodes, bool t, int s_index )
{
    auto make_time = std::chrono::system_clock::now();
    std::cout << "| STIMULIZE " << VERSION << std::endl
              << "| FILE GENERATED BY COMMAND:"  << std::endl << "| ";

    for ( int i = 0; i < argc; i++ )
    {
        std::cout << argv[ i ] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<std::string> inputs      ( v_nodes );
    std::vector<char>        input_values( v_nodes );
    std::vector<bool>        toggle      ( v_nodes );
    
    bool low = false;
    bool high = false;
    
    unsigned int count_to = 1 << v_nodes;
    
    if( s_index == -1 )
    {
        std::cout << "stepsize 5" << std::endl;
    }
    else
    {
        std::cout << "stepsize " << argv[ s_index + 1 ] << std::endl;
    }
    
    a_index++;
    std::cout << "analyzer ";
    for( int i = 0; i < a_nodes; i++ )
    {
        std::cout << argv[ a_index + i ] << " ";
    }
    std::cout << std::endl << std::endl;
    
    v_index++;
    for( int i = 0; i < v_nodes; i++ )
    {
        inputs[ i ] = argv[ v_index + i ];
        input_values[ i ] = 0;
        toggle[ i ] = false;
    }
    
    //default sequence (count through all combinations in gray)
    if( !t )
    {
        unsigned int bit = 1;
        int shift_right = 0;
        
        printState( inputs, v_nodes, 0, "" );
        
        std::cout << "l ";
        for( int n = 0; n < v_nodes; n++ )
        {
            std::cout << inputs[ n ] << " ";
        }
        std::cout << std::endl << "s" << std::endl << std::endl;

        for( unsigned int bnum = 1; bnum < count_to; bnum++ )
        {
            unsigned int gnum = binaryToGray( bnum );

            low = false;
            high = false;

            printState( inputs, v_nodes, gnum, "" );
            
            for( int n = v_nodes - 1; n >= 0; n-- )
            {
                //count in binary
                //if( ( ( bnmum & mask ) >> ( shift_right ) != input_values[ n ] ) )

                //count in gray code
                if( ( gnum & 1 ) != input_values[ n ] )
                {
                    toggle[ n ] = true;
                    
                    if( !low && input_values[ n ] == 1 )
                    {
                        low = true;
                    }
                    else if( !high && input_values[ n ] == 0 )
                    {
                        high = true;
                    }
                }
                else
                {
                    toggle[ n ] = false;
                }

                gnum = gnum >> 1;
            }
            
            if( low )
            {
                std::cout << "l ";
                
                for( int n = 0; n < v_nodes; n++ )
                {
                    if( toggle[ n ] && ( input_values[ n ] == 1 ) )
                    {
                        toggle[ n ] = false;
                        input_values[ n ] = 0;
                        std::cout << inputs[ n ] << " ";
                    }
                }
                
                std::cout << std::endl;
            }
            
            if( high )
            {
                std::cout << "h ";
                
                for( int n = 0; n < v_nodes; n++ )
                {
                    if( toggle[ n ] && ( input_values[ n ] == 0 ) )
                    {
                        toggle[ n ] = false;
                        input_values[ n ] = 1;
                        std::cout << inputs[ n ] << " ";
                    }
                }
                
                std::cout << std::endl;
            }
            
            std::cout << "s" << std::endl << std::endl;
        }
        
        printState( inputs, v_nodes, 0, "" );
        
        //end by making all inputs low
        std::cout << "l ";

        for( int n = 0; n < v_nodes; n++ )
        {
            std::cout << inputs[ n ] << " ";
            input_values[ n ] = 0;
        }
        std::cout << std::endl << "s" << std::endl << std::endl;
    }
    
    //custom test sequence
    else
    {
        std::string arg;
        
        t_index++;
        arg = std::string( argv[ t_index ] );
        for( int n = 0; n < v_nodes; n++ )
        {
            input_values[ n ] = arg[ n ] - 48;
            
            if( !low && input_values[ n ] == 0 )
            {
                low = true;
            }
            else if( !high && input_values[ n ] == 1 )
            {
                high = true;
            }
        }
        
        printState( inputs, v_nodes, 0, arg );
        
        if( low )
        {
            std::cout << "l ";
            
            for( int n = 0; n < v_nodes; n++ )
            {
                if( input_values[ n ] == 0 )
                {
                    std::cout << inputs[ n ] << " ";
                }
            }
            
            std::cout << std::endl;
        }
        
        if( high )
        {
            std::cout << "h ";
            
            for( int n = 0; n < v_nodes; n++ )
            {
                if( input_values[ n ] == 1 )
                {
                    std::cout << inputs[ n ] << " ";
                }
            }
            
            std::cout << std::endl;
        }
        
        std::cout << "s" << std::endl << std::endl;

        for( int i = 1; i < t_nodes; i++ )
        {
            low = false;
            high = false;
            
            arg = std::string( argv[ t_index + i ] );
            for( int n = 0; n < v_nodes; n++ )
            {
                if( (arg[ n ] - 48) != input_values[ n ] )
                {
                    toggle[ n ] = true;
                    
                    if( !low && input_values[ n ] == 1 )
                    {
                        low = true;
                    }
                    else if( !high && input_values[ n ] == 0 )
                    {
                        high = true;
                    }
                }
                else
                {
                    toggle[ n ] = false;
                }
            }
            
            printState( inputs, v_nodes, 0, arg );
            
            if( low )
            {
                std::cout << "l ";
                
                for( int n = 0; n < v_nodes; n++ )
                {
                    if( toggle[ n ] && input_values[ n ] == 1 )
                    {
                        toggle[ n ] = false;
                        input_values[ n ] = 0;
                        std::cout << inputs[ n ] << " ";
                    }
                }
                
                std::cout << std::endl;
            }
            
            if( high )
            {
                std::cout << "h ";
                
                for( int n = 0; n < v_nodes; n++ )
                {
                    if( toggle[ n ] && input_values[ n ] == 0 )
                    {
                        toggle[ n ] = false;
                        input_values[ n ] = 1;
                        std::cout << inputs[ n ] << " ";
                    }
                }
                
                std::cout << std::endl;
            }
            
            std::cout << "s" << std::endl << std::endl;
        }
    }
}

void show_usage()
{
    std::cerr << std::endl
              << "Usage:" << std::endl << std::endl
              << "stimulize -a {node_name ..} -v {node_name ..} "
              << "[-s <step_val>] [-t {test_seq ..}]" << std::endl << std::endl
    
              << "Required:" << std::endl << std::endl
              << "  -a,--analyzer {node_name ..}          "
              << "nodes to view on the analyzer window." << std::endl
              << "  -v,--vector {node_name ..}            "
              << "nodes to vectorize." << std::endl << std::endl
    
              << "Optional:" << std::endl << std::endl
              << "  -s,--stepsize <step_val>              "
              << "set the stepsize to <step_val>, default is 5ns." << std::endl
              << "  -t,--test {test_seq ..}               "
              << "binary sequences to test on vector." << std::endl
              << "                                        "
              << "default is 0 to (2^[num_of_vector_nodes])-1." << std::endl
              << std::endl
    
              << "Examples:" << std::endl << std::endl
              << "  stepsize: 5ns, analyzer: [A,B,C,OUT], vector: [A,B,C], from [0,0] to [1,1,1]" << std::endl << std::endl
              << "$ stimulize -a A B C OUT -v A B" << std::endl << std::endl
              << "  stepsize: 50ns, analyzer: [I0,I1,I2,I3,O0,O1], vector: [I0,I1,I2,I3] = [0,0,0,0] [1,1,1,0] [0,0,1,1] [1,1,1,1]" << std::endl
              << "$ stimulize -a I0 I1 I2 I3 O0 O1 -v I0 I1 I2 I3 0000  1110  0011 1111 > test.cmd" << std::endl << std::endl
              << "If this program isinstalled on your system, use " << std::endl
              << "$ man stimulize"   << std::endl
              << "for more / better formatted details" << std::endl << std::endl;
}

int main( int argc, char** argv )
{
    int analyzer_index = -1;
    int analyzer_nodes = 0;
    bool analyzer = false;
    
    int vector_index = -1;
    int vector_nodes = 0;
    bool vector = false;
   
    int stepsize_index = -1;
    int stepsize_nodes = 0;
    bool stepsize = false;
    
    int test_index = -1;
    int test_nodes = 0;
    bool test = false;
    
    std::string flag = "";
    std::string arg;
    int count;
    
    bool error = false;
    
    if( argc == 1 )
    {
        show_usage( );
        exit( 1 );
    }
    
    for( int i = 1; i < argc; i++ )
    {
        arg = std::string( argv[ i ] );
        if( arg == "-a" || arg == "--analyzer" )
        {
            analyzer_index = i;
            
            if( analyzer == true )
            {
                std::cerr << "--analyzer, -a: Ocurred too many times" << std::endl;
                error = true;
            }
            
            analyzer = true;
            
            if( flag == "v" )
            {
                vector_nodes = count;
            }
            else if( flag == "t" )
            {
                test_nodes = count;
            }
            else if( flag == "s" )
            {
                stepsize_nodes = count;
            }
            
            flag = "a";
            count = -1;
        }
        else if( arg == "-v" || arg == "--vector" )
        {
            vector_index = i;
            
            if( vector == true )
            {
                std::cerr << "--vector, -v: Ocurred too many times" << std::endl;
                error = true;
            }
            
            vector = true;
            
            if( flag == "a" )
            {
                analyzer_nodes = count;
            }
            else if( flag == "t" )
            {
                test_nodes = count;
            }
            else if( flag == "s" )
            {
                stepsize_nodes = count;
            }
            
            flag = "v";
            count = -1;
        }
        else if( arg == "-t" || arg == "--test" )
        {
            test_index = i;
            
            if( test == true )
            {
                std::cerr << "--test, -t: Ocurred too many times" << std::endl;
                error = true;
            }
            
            test = true;
            
            if( flag == "a" )
            {
                analyzer_nodes = count;
            }
            else if( flag == "v" )
            {
                vector_nodes = count;
            }
            else if( flag == "s" )
            {
                stepsize_nodes = count;
            }
            
            flag = "t";
            count = -1;
        }
        else if( arg == "-s" || arg == "--stepsize" )
        {
            stepsize_index = i;
            
            if( stepsize == true )
            {
                std::cerr << "--stepsize, -s: Ocurred too many times" << std::endl;
                error = true;
            }
            
            stepsize = true;
            
            if( flag == "a" )
            {
                analyzer_nodes = count;
            }
            else if( flag == "v" )
            {
                vector_nodes = count;
            }
            else if( flag == "t" )
            {
                test_nodes = count;
            }
            
            flag = "s";
            count = -1;
        }
        else if( arg.find( '-' ) != -1 )
        {
            show_usage();
            exit( 1 );
        }
        
        count++;
    }
    
    if( flag == "v" )
    {
        vector_nodes = count;
    }
    else if( flag == "t" )
    {
        test_nodes = count;
    }
    else if( flag == "a" )
    {
        analyzer_nodes = count;
    }
    else if( flag == "s" )
    {
        stepsize_nodes = count;
    }
    
    if( !analyzer )
    {
        std::cerr << "--analyzer, -a: Flag required." << std::endl;
        error = true;
    }
    
    if( !vector )
    {
        std::cerr << "--vector, -v: Flag required." << std::endl;
        error = true;
    }
    
    if( analyzer && analyzer_nodes == 0 )
    {
        std::cerr << "--analyzer, -a: No nodes submitted." << std::endl;
        error = true;
    }
    
    if( vector && vector_nodes == 0 )
    {
        std::cerr << "--vector, -v: No nodes submitted." << std::endl;
        error = true;
    }
    
    bool non_binary  = false;
    
    if( test )
    {
        if( test_nodes == 0 )
        {
            std::cerr << "--test, -t: No test sequence submitted." << std::endl;
            error = true;
        }
    
        for( int i = 0; i < test_nodes; i++ )
        {
            arg = std::string( argv[ (test_index + 1) + i ] );
            if( arg.length() != vector_nodes )
            {
                std::cerr << "--test, -t: Sequence (" << i+1 << ")";
                std::cerr << " does not contain (" << vector_nodes << ") values: ";
                std::cerr << arg << std::endl;
                error = true;
            }
            
            non_binary = false;
            
            for( int c = 0; c < arg.length(); c++ )
            {
                if( arg[ c ] != '0' && arg[ c ] != '1' )
                {
                    if( !non_binary )
                    {
                        std::cerr << "--test, -t: Sequence (" << i+1 << ")";
                        std::cerr << " contains non-binary character(s): ";
                        
                        non_binary = true;
                    }
                    
                    std::cerr << arg[c];
                    error = true;
                }
            }
            
            if( non_binary )
            {
                std::cerr << std::endl;
            }
        }
    }
    
    if(  stepsize && stepsize_nodes == 0 )
    {
        std::cerr << "--stepsize, -s: No value submitted." << std::endl;
        error = true;
    }
    
    if( stepsize && stepsize_nodes > 1 )
    {
        std::cerr << "--stepsize, -s: More than one value was passed in." << std::endl;
        error = true;
    }
    
    if( error )
    {
        std::cerr << "If this program isinstalled on your system, use " << std::endl
                  << "$ man stimulize"   << std::endl
                  << "for more details" << std::endl;
        exit( 1 );
    }
    
    generateCommands( argv, argc, analyzer_index, analyzer_nodes, vector_index, vector_nodes,
                     test_index, test_nodes, test, stepsize_index );
    
    return 0;
}
