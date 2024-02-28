d = {
'PiS' : 8051935,
'KO' : 5060355,
'SLD' : 2319946,
'PSL' : 1578523,
'Konfa' : 1256953,
'KWW' : 32094
}

def find_next(d1, d2):
    maks = -1
    for key in d1:
        if d1[key] / (d2[key] + 1) > maks:
            maks = d1[key] / (d2[key] + 1)
            res = key
    return res, maks


def dhondt(wyniki, miejsca):
    aux = wyniki.copy()
    res = {}
    for key in wyniki:
        res[key] = 0
    
    while miejsca > 0:
        next_key, next_val = find_next(wyniki, res)
        res[next_key] += 1
        aux[next_key] = next_val
        miejsca -= 1

    return res

def dhondtv2(wyniki, miejsca):
    tab = []
    res = {}
    for key in wyniki:
        res[key] = 0
    list_res = list(res.items())
    for key in wyniki:
        val = wyniki[key]
        pom = [key]
        for i in range(miejsca):
            pom.append(val / (i+1))
        tab.append(pom)
        
    for i in range(miejsca):
        maks = 0
        for j in range(len(tab)):
            if tab[j][1] > maks:
                maks = tab[j][1]
                index = j
        tab[index].pop(1)
        res[tab[index][0]] += 1
    return res


# Kod do weryfikacji z githuba
def dhont(nSeats,votes,verbose=False):
    """
    nSeats is the number of seats
    votes is a dictionary with the key:value {'party':votes}
    verbose is an option to print designation info
    """
    t_votes=votes.copy()
    seats={}
    for key in votes: seats[key]=0
    while sum(seats.values()) < nSeats:
        max_v= max(t_votes.values())
        next_seat=list(t_votes.keys())[list(t_votes.values()).index(max_v)]
        if next_seat in seats:
            seats[next_seat]+=1
        else:
            seats[next_seat]=1
        
        if verbose:
            print("{} Escaño: {}".format(sum(seats.values()),next_seat))
            for key in t_votes:
                print("\t{} [{}]: {:.1f}".format(key,seats[key],t_votes[key]))
            print("\b")
        t_votes[next_seat]=votes[next_seat]/(seats[next_seat]+1)
    return seats

print(dhondt(d, 460))
print(dhondtv2(d, 460))
print(dhont(460, d))