##########
## The oracle works as follows:  
## # 1 # Generate the ASTs using the results from the prepare_grammar and enumerate scripts  
## # 2 # Run the generated ASTs (as in the Solver class in /smt_solver_testing/Oracle.py) on a prebuilt jsonlint binary
## # 3 # In theory all of the generated ASTs should be valid, so keep going until all of the enumerated ASTs are run. 
## # 4 # Output the result "num_valid, num_invalid, invalid ASTs".  
###########
## Also write a test harness that allows to run the enumeration multiple times with ever-increasing depths until an invalid AST is found.

import time
import shutil
import logging
import subprocess
import multiprocessing as mp

from enum import Enum
from smt_solver_testing.Logger import (
    log_invalid_mutant,
    log_soundness_trigger
)

def plain(cli):
    plain_cli = ""
    for token in cli.split(" "):
        plain_cli += token.split("/")[-1]
    return escape(plain_cli)


def escape(s):
    s = s.replace(".", "")
    s = s.replace("=", "")
    return s


def report_diff(
    scratchfile,
    bugtype,
    ref_cli,
    ref_stdout,
    ref_stderr,
    sol_cli,
    sol_stdout,
    sol_stderr,
    args
):
    plain_cli = plain(sol_cli)
    # format: <solver><{crash,wrong,invalid_model}><seed>.<random-str>.smt2
    report = "%s/%s-%s-%s.json" % (
        args.bugfolder,
        bugtype,
        plain_cli,
        scratchfile.split("/")[-1].split(".")[-2]
    )
    try:
        shutil.copy(scratchfile, report)
    except Exception:
        print("error: couldn't copy scratchfile to bugfolder.")
        exit(1)

    logpath = "%s/%s-%s-%s.output" % (
        args.bugfolder,
        bugtype,
        plain_cli,
        scratchfile.split("/")[-1].split(".")[-2]
    )
    with open(logpath, "w") as log:
        log.write("*** REFERENCE \n")
        log.write("command: " + ref_cli + "\n")
        log.write("stderr:\n")
        log.write(ref_stderr)
        log.write("stdout:\n")
        log.write(ref_stdout)
        log.write("\n\n*** INCORRECT \n")
        log.write("command: " + sol_cli + "\n")
        log.write("stderr:\n")
        log.write(sol_stderr)
        log.write("stdout:\n")
        log.write(sol_stdout)
    return report


def report(scratchfile, bugtype, cli, stdout, stderr, args):
    plain_cli = plain(cli)
    # format: <solver><{crash,wrong,invalid_model}><seed>.smt2
    report = "%s/%s-%s-%s.smt2" % (
        args.bugfolder,
        bugtype,
        plain_cli,
        scratchfile.split("/")[-1].split(".")[-2]
    )
    try:
        shutil.copy(scratchfile, report)
    except Exception as e:
        # print("error: couldn't copy scratchfile to bugfolder.")
        logging.error("error: couldn't copy scratchfile to bugfolder.")
        exit(1)
    logpath = "%s/%s-%s-%s.output" % (
        args.bugfolder,
        bugtype,
        plain_cli,
        scratchfile.split("/")[-1].split(".")[-2]
    )
    with open(logpath, "w") as log:
        log.write("command: " + cli + "\n")
        log.write("stderr:\n")
        log.write(stderr)
        log.write("stdout:\n")
        log.write(stdout)
    return report


def grep_result(stdout: str, exitcode: int):
    """
    Grep the result from the stdout of a solver.
    """
    result = ValidatorResult()
    lines = stdout.splitlines()
    
    if len(lines) > 0 and exitcode == 0: # A valid json output will always start with an opening {
        result.append(ValidatorQueryResult.VALID)
    elif len(lines) == 1 and exitcode == 1:
        result.append(ValidatorQueryResult.INVALID)
    else:
        result.append(ValidatorQueryResult.UNKNOWN)
    return result


def in_list(stdout, stderr, lst):
    stdstream = stdout + " " + stderr
    for err in lst:
        if err in stdstream:
            return True
    return False

class Validator:
    def __init__(self, cli):
        self.cli = cli

    def validate(self, file, timeout, debug=False):
        total_time = -1
        try:
            t1 = time.time()
            mem_limit = 1048576
            cmd = 'ulimit -Sv {};timeout -s 9 64'.format(mem_limit) + self.cli + " "+ file
            
            if debug:
                print(cmd)
            output = subprocess.run(
                cmd,
                timeout=timeout,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                shell=True,
            )
            total_time = time.time() - t1

        except subprocess.TimeoutExpired as te:
            if te.stdout and te.stderr:
                stdout = te.stdout.decode()
                stderr = te.stderr.decode()
            else:
                stdout = ""
                stderr = ""
            return stdout, stderr, 137, -1

        except ValueError:
            stdout = ""
            stderr = ""
            return stdout, stderr, 0, -1

        except FileNotFoundError:
            print('error: validator "' + cmd[0] + '" not found', flush=True)
            exit(ERR_USAGE)

        stdout = output.stdout.decode()
        stderr = output.stderr.decode()
        returncode = output.returncode

        if debug:
            print("output: " + stdout + "\n" + stderr)

        return stdout, stderr, returncode, total_time 


def sr2str(sol_res):
    if sol_res == ValidatorQueryResult.VALID:
        return "valid"
    if sol_res == ValidatorQueryResult.INVALID:
        return "invalid"
    if sol_res == ValidatorQueryResult.UNKNOWN:
        return "unknown"


class ValidatorResult:
    """
    Class to store the result of multiple validation queries.
    :lst a list of multiple "ValidatorQueryResult" items
    """

    def __init__(self, result=None):
        self.lst = []
        if result:
            self.lst.append(result)

    def append(self, result):
        self.lst.append(result)

    def equals(self, rhs):
        if type(rhs) == ValidatorQueryResult:
            return len(self.lst) == 1 and self.lst[0] == rhs
        elif type(rhs) == ValidatorResult:
            if len(self.lst) != len(rhs.lst):
                return False
            for index in range(0, len(self.lst)):
                if (
                    self.lst[index] != ValidatorQueryResult.UNKNOWN
                    and rhs.lst[index] != ValidatorQueryResult.UNKNOWN
                    and self.lst[index] != rhs.lst[index]
                ):
                    return False
            return True
        else:
            return False

    def __str__(self):
        s = sr2str(self.lst[0])
        for res in self.lst[1:]:
            s += "\n" + sr2str(res)
        return s


class ValidatorQueryResult(Enum):
    """
    Enum storing the result of a single validator query.
    """

    VALID = 0 # Validator returns a formatted version of the input  
    INVALID = 1 # Validator returns an error  
    UNKNOWN = 2 # Garbage

def testJson(fn, args):
    """
    returns num_bugs, num_ineffective_tests
    """
    oracle = ValidatorResult(ValidatorQueryResult.UNKNOWN)
    num_ineffective_tests = 0
    num_bugs = 0

    # We assume that the user inputs 2 CLIS:
    # 1 - the reference CLI
    # 2 - the tested CLI
    assert len(args.CLIS) == 2

    ref_cli = args.CLIS[0]
    test_cli = args.CLIS[1]

    # Create the two json validator objects
    ref_val = Validator(ref_cli)
    test_val = Validator(test_cli)

    # Feed the inputs to the two validators
    ref_stdout, ref_stderr, ref_exitcode, ref_total_time = ref_val.validate(
        fn, args.timeout
    )
    test_stdout, test_stderr, test_exitcode, test_total_time = test_val.validate(
        fn, args.timeout
    )

    # Check the results against the two validators
    oracle = grep_result(ref_stdout, ref_exitcode)
    result = grep_result(test_stdout, test_exitcode)

    # Check that the validators accepted the inputs
    if oracle.equals(result) and not oracle.equals(ValidatorQueryResult.VALID):
        log_invalid_mutant(fn, ref_cli)
        log_invalid_mutant(fn, test_cli)
        num_ineffective_tests += 1
    # Check if the two test results aren't equal
    elif not oracle.equals(result):
        path = report_diff(
            fn,
            "incorrect",
            ref_cli,
            ref_stdout,
            ref_stderr,
            test_cli,
            test_stdout,
            test_stderr,
            args
        )
        log_soundness_trigger(fn)
        num_bugs += 1

    return num_bugs, num_ineffective_tests
