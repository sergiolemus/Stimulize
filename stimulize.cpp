#include <iostream>
#include <string>

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

// ./STIMULIZE -analyzer A B OUT -v A B

// EXAMPLE: (IN_1,IN_2,IN_3) AS INPUTS AND (OUT_1,OUT_2) AS OUTPUTS
// (IN_1,IN_2,IN_3) WILL GO RESPECTIVELY FROM (0,0,0) -> (0,0,1) -> ... -> (1,1,0) -> (1,1,1)
// WITH A STEPSIZE OF 10ns

//./STIMULIZE -a IN_1 IN_2 IN_3 OUT_A OUT_B -v IN_1 IN_2 IN_3 -s 10

// FUNCTION TO GENERATE VECTORIZED INPUTS
void generateCommands( char** argv, int a_index, int a_nodes, int v_index, int v_nodes,
                      int t_index, int t_nodes, bool t, int s_index )
{
    std::string* inputs = new std::string[ v_nodes ];
    char* input_values = new char[ v_nodes ];
    bool* toggle = new bool[ v_nodes ];
    
    bool low = false;
    bool high = false;
    
    unsigned int count_to = 2;
    
    for( int i = 1; i < v_nodes; i++ )
    {
        count_to = count_to << 1;
    }
    
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
    }   std::cout << std::endl;
    
    v_index++;
    for( int i = 0; i < v_nodes; i++ )
    {
        inputs[ i ] = argv[ v_index + i ];
        input_values[ i ] = 0;
        toggle[ i ] = false;
    }
    
    if( !t )
    {
        unsigned int bit = 1;
        int shift_left = 0;
        
        std::cout << "l ";
        for( int n = 0; n < v_nodes; n++ )
        {
            std::cout << inputs[ n ] << " ";
        }   std::cout << std::endl;
        
        std::cout << "s" << std::endl;
        
        for( unsigned int i = 1; i < count_to; i++ )
        {
            low = false;
            high = false;
            
            bit = 1;
            shift_left = 0;
            
            for( int n = v_nodes - 1; n >= 0; n-- )
            {
                if( ( ( i & bit ) >> shift_left ) != input_values[ n ] )
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

                bit = bit << 1;
                shift_left++;
            }
            
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
            
            std::cout << "s" << std::endl;
        }
        
        std::cout << "l ";
        for( int n = 0; n < v_nodes; n++ )
        {
            std::cout << inputs[ n ] << " ";
        }   std::cout << std::endl;
        
        std::cout << "s" << std::endl;
    }
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
        
        std::cout << "s" << std::endl;
        
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
            
            std::cout << "s" << std::endl;
        }
    }
    
    delete[] inputs;
    delete[] input_values;
    delete[] toggle;
}

void show_usage()
{
    std::cerr << std::endl;
    std::cerr << "Usage:" << std::endl << std::endl;
    std::cerr << "stimulize -a {node_name ..} -v {node_name ..} ";
    std::cerr << "[-s <step_val>] [-t {test_seq ..}]" << std::endl << std::endl;
    
    std::cerr << "Required:" << std::endl << std::endl;;
    std::cerr << "  -a,--analyzer {node_name ..}          ";
    std::cerr << "nodes to view on the analyzer window." << std::endl;
    std::cerr << "  -v,--vector {node_name ..}            ";
    std::cerr << "nodes to vectorize." << std::endl << std::endl;
    
    std::cerr << "Optional:" << std::endl << std::endl;
    std::cerr << "  -s,--stepsize <step_val>              ";
    std::cerr << "set the stepsize to <step_val>, default is 5ns." << std::endl;
    std::cerr << "  -t,--test {test_seq ..}               ";
    std::cerr << "binary sequences to test on vector." << std::endl;
    std::cerr << "                                        ";
    std::cerr << "default is 0 to (2^[num_of_vector_nodes])-1." << std::endl;
    std::cerr << std::endl;
    
    std::cerr << "Examples:" << std::endl << std::endl;
    std::cerr << "  -a A B OUT -v A B" << std::endl;
    std::cerr << "  stepsize: 5ns, analyzer: [A,B,OUT], vector: [A,B], from [0,0] to [1,1]" << std::endl << std::endl;
    std::cerr << "  -a I1 I2 I3 O1 O2 -v I1 I2 I3 -s 50 -t 000 010 111" << std::endl;
    std::cerr << "  stepsize: 50ns, analyzer: [I1,I2,I3,O1,O2], vector: [I1,I2,I3] = [0,0,0] [0,1,0] [1,1,1]" << std::endl;
    std::cerr << std::endl;
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
        exit( 1 );
    }
    
    generateCommands( argv, analyzer_index, analyzer_nodes, vector_index, vector_nodes,
                     test_index, test_nodes, test, stepsize_index );
    
    return 0;
}

