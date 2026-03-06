# Lab2

## Lab2A Student-College Matching

This is a multi-to-one matching problem, where we have a set of students and a set of colleges. Each student has a satisfaction score for each college (possibly negative, and for those colleges, student will not go to them even if they are matched to them, thus we could assume that the student will not apply those colleges).

Then it is the classical Gale-Shapley algorithm, where students are the proposers and colleges are the receivers. The algorithm is simple, we put all student in the free queue, and each student will apply to the college with the highest satisfaction score. Each college will keep the top k students (where k is the capacity of the college) and reject the rest. The rejected students will be put back to the free queue, and they will apply to the next college in their preference list. The algorithm will terminate when there is no free student.

There are three situation when a student apply to a college: 
- The college has not reached its capacity, then the student will be accepted if the students score is not negative, otherwise the student will be rejected.
- The college has reached its capacity, but the student has a higher score than the lowest score student in the college, then the student will be accepted and the lowest score student will be rejected.
- The college has reached its capacity, and the student has a lower score than the lowest score student in the college, then the student will be rejected.

Repeating the above process until there is no free student, we will get a stable matching.

## Lab2B Codeforces 1147F: Zigzag Game (Codeforces 3500+, Luogu NOI/NOI+/CTSC)

See my blog on Zhihu.