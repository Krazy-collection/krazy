//dpointer issue with no line ("missing dpointer in classes: ClassWithInlinedPrivateMethods")

class FOO_EXPORT ClassWithInlinedPrivateMethods {
private:

    bool mAttribute1; //dpointer

    void method1();

    void method2(int argument1);

    bool mAttribute2; //dpointer

    void method3(int argument1, int argument2, int argument3);

    void method4(int argument1,
                 int argument2,
                 int argument3);

    void method5() const;

    void method6(int argument1,
                 int argument2,
                 int argument3) const;

    bool mAttribute3; //dpointer

    void inlinedMethod1() {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    void inlinedMethod2(int argument1) {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    bool mAttribute4; //dpointer

    void inlinedMethod2(int argument1, int argument2, int argument3) {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    void inlinedMethod4(int argument1,
                        int argument2,
                        int argument3) {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    void inlinedMethod5() const {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    void inlinedMethod6(int argument1,
                        int argument2,
                        int argument3) const {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    bool mAttribute5; //dpointer

    template<typename T>
    void templatedMethod1() {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    template<typename T>
    QList< QList<T> > templatedMethod1() {
        if (someCondition) {
            return;
        }

        int variable;
        doThings();
    }

    bool mAttribute6; //dpointer

};
