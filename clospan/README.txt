CloSpan: Mining Closed Sequential Patterns

Author: Xifeng Yan, University of Illinois at Urbana-Champaignd

The program is built upon PrefixSpan source code, "PrefixSpan:
Mining Sequential Patterns Efficiently by Prefix-Projected Pattern
Growth" Jian Pei, Jiawei Han, Behzad Mortazavi-Asl, Helen Pinto,
Qiming Chen, Umeshwar Dayal, Mei-Chun Hsu (ICDE'2001)

Contact: xyan@cs.uiuc.edu

Reference: "X. Yan, J. Han, R. Afshar, CloSpan: Mining Closed
Sequential Patterns in Large Databases, Proc. 2003 SIAM Int. Conf.
Data Mining (SDM'03)", 166 - 177, 2003.

NOTE:
   For compiling, type "make" under clospan directory.

How-To:
    clospan filename min_sup num_of_labels

    Parameters: (1) filename, your binary data (2) min_sup, the
    minimum frequency of patterns (3) num_of_labels, the number of
    distinct item labels.

    Example:
        clospan D10N1B.data 0.1 1000
        It mines all frequent sequences from "D10N1B.data", each of which
        should appear in at least 10% of the sequences in the dataset. 1000
        means there are 1000 different symbols in this dataset.


Input Format:
    1. The input is a set of sequences; each sequence has the following
    format
    <(item_11, item_12, ..., item_1n)(item_21, item_22, ... item_2m)...>
      ------------------------------  -----------------------------
           transaction 1                    transaction 2 ......

    Example:
        <(ab)(c)(d)>
        <(e)(acfh)>
        ...

    The input is stored in a binary file, we use a 4-byte integer "-1" to
    separate transactions in each sequence and another 4-byte integer "-2"
    to separate sequences in a dataset.  Each of items is encoded using a
    4-byte integer. For example, <(ab)(c)(d)><(e)(acfh)> is stored as
            ab-1c-1d-1-2e-1acfh-1-2
    where each symbol is a 4-byte integer and all of them are concatenated
    together.

Output:
    Program status as it is executing and the final results (such as timing)
    are printed to stdout (console).
    The discovered patterns are stored in a file named "ClosedPatterns",
    which is in a format of plain text.
    The first column in the output file shows the discovered patterns.
    The second column in the output file is the number of times that a
    pattern appears in the dataset.

