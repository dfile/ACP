#Global list of all curvatures in the packing
CURVELIST = set()

# input: quad (a quadruple of curvatures), limit is the maximum curvature allowable
# output: solutions (list of transformed quadruples)
# function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
# check to make sure the new curvature is under the limit
def transform(quad, limit):
    solutions = []
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    transformed = [0, 0, 0, 0]
    transformed[0]= -a + (2 * (b + c + d))
    transformed[1]= -b + (2 * (a + c + d))
    transformed[2]= -c + (2 * (a + b + d))
    transformed[3]= -d + (2 * (a + b + c))
    
    for i in range(4):
        if quad[i] < transformed[i]< limit:
            prime = quad[:]
            prime[i] = transformed[i]
            solutions.append(prime)
            CURVELIST.add( transformed[i])
            
    return solutions


# input: root (the initial quadruple that defines the packing), limit (arbitrary limit on curvatures we don't want to go above)
# output: ancestors (list of quadruples all below the limit)
def fuchsian(root, limit):
    ancestors = []
    ancestors.append(root)
    for parent in ancestors:
        nextGen = transform(parent , limit)
        ancestors.extend(nextGen)
    return #ancestors

# input: quadList (list of quadruples)
# output: actual (list of all the values of the input list)
# creates a list of unique curvatures
def valuesOf(quadList):
    possible = set()
    for ruple in quadList:
        for value in ruple:
            possible.add(value)
    return possible

# input: quad (a quadruple), orbit (list of admissible quadruples)
# output: solutions (latest generation of admissible quadruples), orbit (list of updated admissible quadruples)
# define orbit: An orbit of a circle packing is all quadruples mod 24 which any quadruple in the packing will be equivalent to, no matter how far out you go. We're interested in finding orbits of arbitrary packings so we can check curvatures in them against it
def transformOrbit(quad, orbit):
    solutions = []
    family = [-1, -1, -1 ,-1]
    a, b, c, d = quad[0], quad[1], quad[2], quad[3]
    
    # the four matrices
    family[0] = [(-a + (2 * (b + c + d))) % 24, b % 24, c % 24, d % 24]
    family[1] = [a % 24, (-b + (2 * (a + c + d))) % 24, c % 24, d % 24]
    family[2] = [a % 24, b % 24, (-c + (2 * (a + b + d))) % 24, d % 24]
    family[3] = [a % 24, b % 24, c % 24, (-d + (2 * (a + b + c))) % 24]
    # print ("original orbit: ", orbit)
    for kid in family:
        if kid in orbit:
            pass
        else:
            orbit.append(kid)
            solutions.append(kid)
    return solutions, orbit

# input: seed (vector which represents the quadruple which defines the packing)
# output: orbit (list of all admissible quadruples [see above])
# genealogy is the host function which finds the complete orbit
def genealogy(seed):
    ancestors, orbit, modSeed = [], [], [0, 0, 0, 0]
    for i in range(4):
        # print("seed[i] = " , seed[i])
        modSeed[i] = (seed[i] % 24)
        # print("modSeed[i] = " , modSeed[i]);
    ancestors.append(modSeed)
    orbit.append(modSeed)
    for parent in ancestors:
        newGeneration, orbit = transformOrbit(parent, orbit)
        # print("orbit: ", orbit)
        # print("newGeneration: ", newGeneration)
        ancestors.extend(newGeneration)
    return orbit

# input: valList (admissible values of the packing), top (arbitrary limit)
# output: could (list of admissible values up to our arbitrary limit)
def path(valList, top):
    could = set()
    i = 0
    while i < top:
        for poss in valList:
            if (i % 24) == poss:
                could.add(i)
                break
        i += 1
    return could

# input: root (quadruple which defines the packing), cap (arbitrary limit)
# output: gone (list of admissible values that DO NOT appear in the packing, though they should)
# host function for the last block of code, finds gone, the list we're looking for
def seek(root, cap):
    #vecRoot = vector(ZZ, root)
    fuchsian(root, cap)
    valuesPack = valuesOf(small)
    admissible = genealogy(root)
    #print("genealogy results:")
    #print(admissible)
    valuesOrbit = valuesOf(admissible)
    valuesGlobal = path(valuesOrbit, cap)
    nope = valuesGlobal.difference( valuesPack )
    missing =  list(nope)
    missing.sort()
    return missing