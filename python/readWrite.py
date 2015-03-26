import string

def transform(quad):
    solutions = []
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    if (-a + (2 * (b + c + d))) > a:
        aPrime = [(-a + (2 * (b + c + d))), b, c, d]
        solutions.append(aPrime)
    if (-b + (2 * (a + c + d))) > b:
        bPrime = [a, (-b + (2 * (a + c + d))), c, d]
        solutions.append(bPrime)
    if (-c + (2 * (a + b + d))) > c:
        cPrime = [a, b, (-c + (2 * (a + b + d))), d]
        solutions.append(cPrime)
    if (-d + (2 * (a + b + c))) > d:
        dPrime = [a, b, c, (-d + (2 * (a + b + c)))]
        solutions.append(dPrime)
    return solutions



def seek( root, cap, maxCounter):
    '''
    root: a list of four integers, the root quadruple for our packing
    cap: largest possible curvature we will use
    maxCounter: maximum number of quadruples in the stack
    '''
    FILESTRING = str(root) + "_" + str(cap) + "_" + str(maxCounter) + ".txt"
    
    newGeneration = transform( root )
    
    quadList = []
    
    with open(FILESTRING, 'w') as infile:
        for quad in newGeneration:
            for value in quad:
                infile.write( str(value)+ ' ' )
            infile.write( '\n' )
    
    
    with open(FILESTRING, 'r') as infile:
        for line in infile:
            vec = string.split( line )
            for i in range(4):
                vec[i] = int(vec[i])
            quadList.append(vec)
            
    print quadList

        
    

