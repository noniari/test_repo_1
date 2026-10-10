#pragma once

struct kid_t {
    int id;
    int age;
};

struct kidgroup_t {
    int nkid;
    int ngroup;
};

struct answer_t {
    int ngroups;
    kidgroup_t* mapping;
};

answer_t funparty(kid_t* kids, int nkids);