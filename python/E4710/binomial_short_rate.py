import math 
import decimal 


R_0 = 0.04
U = 1.1
D = 0.9
Q = 0.5
P= (1-Q)


def zero_coupon_bond(m, R_0 , U, D, Q):
	P= 1-Q
	current= list()
	prev = list()
	t = m 
	while t>=0:
		i = 0 
		while i<= t:
			if t==m :
				current.append(1)
			else :
				current[i] = (prev[i]*Q + prev[i+1] * P )/(1+ R_0* pow(U, i) * pow(D, t-i) )
			i +=1
		#print "time ", t, current 
		prev = current 
		t-=1

	return prev[0]


def zero_coupon_bond_future(m, e, R_0, U, D, Q):
	P= 1-Q
	current= list()
	prev = list()
	t = m 
	while t>=0:
		i = 0 
		while i<= t:
			if t==m :
				current.append(1)
			elif t<e :
				current[i] = prev[i]*Q + prev[i+1] * P 
			else :
				current[i] = (prev[i]*Q + prev[i+1] * P )/(1+ R_0* pow(U, i) * pow(D, t-i) )
			i +=1
		#print "time ", t, current 
		prev = current 
		t-=1

	return prev[0]

def swap_rate(r, B, n, U, D, Q):
	fix_leg_val =0
	for t in range(1,n):
		fix_leg_val += zero_coupon_bond(B+t, r, U, D, Q)
	bp_B = zero_coupon_bond(B, r, U, D, Q)
	bp_E = zero_coupon_bond(B+n, r, U, D ,Q)
	return ((bp_B-bp_E)/fix_leg_val)


def swap_rate_future(r, e ,B, n, U, D, Q):
	fix_leg_val =0
	for t in range(1,n):
		fix_leg_val += zero_coupon_bond_future(B+t,e, r, U, D, Q)
	bp_B = zero_coupon_bond_future(B, e,r, U, D, Q)
	bp_E = zero_coupon_bond_future(B+n,e, r, U, D ,Q)
	return ((bp_B-bp_E)/fix_leg_val)


def american_put_zcp(R_0, m, e, k, U, D, Q):
	P= 1-Q
	current= list()
	prev = list()
	
	cur_put = list()
	prev_put = list()

	t = m 
	while t>=0:
		i = 0 
		while i<= t:
			if t==m :
				current.append(1)
			else :
				current[i] = (prev[i]*Q + prev[i+1] * P )/(1+ R_0* pow(U, i) * pow(D, t-i) )
			
			if t==e:
				cur_put.append(max(0, k-current[i]))
			elif t<e:
				cp =  (prev_put[i]*Q + prev_put[i+1] * P )/(1+ R_0* pow(U, i) * pow(D, t-i))
				if k-current[i] > cp:
					print "Early Exec", t, i, current[i]
				cur_put[i] = max(0,k-current[i],cp)
			i +=1
		print "time ", t, cur_put 
		prev = current 
		prev_put = cur_put
		t-=1

	return prev_put[0]





#==== Problem 1 ================================
for r in (0.05, 0.055, 0.06, 0.065, 0.07):
	bp = zero_coupon_bond(5, r, U, D, Q)
	print r, bp, pow(1.0/bp , 1.0/5)-1

#==== Problem 2
for r in (0.05, 0.055, 0.06, 0.065, 0.07):
	print r, swap_rate(r, 2, 5, 1.1, 0.9, 0.5)

print "======swap(1)"
for r in (0.05, 0.055, 0.06, 0.065, 0.07):
        print r, 0.5*swap_rate(r*1.1, 1,5,1.1,0.9,0.5) + 0.5*swap_rate(r*0.9, 1,5,1.1,0.9,0.5)


print "======swap(2)"
for r in (0.05, 0.055, 0.06, 0.065, 0.07):
        print r, 0.5*(0.5*swap_rate(r*1.1*1.1, 0,5,1.1,0.9,0.5) + 0.5*swap_rate(r*1.1*0.9, 0,5,1.1,0.9,0.5))\
		+ 0.5*(0.5*swap_rate(r*0.9*1.1, 0,5,1.1,0.9,0.5) + 0.5*swap_rate(r*0.9*0.9, 0,5,1.1,0.9,0.5))\




future_5_3 = zero_coupon_bond_future(5,3,0.06,1.1,0.9, 0.5)
print future_5_3
print american_put_zcp(0.06, 5, 3, future_5_3,1.1, 0.9, 0.5)

