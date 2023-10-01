/**
 * A {@code VMInstruction} represents an instruction that the physical CPU can run.
 */
@FunctionalInterface
public interface VMInstruction {
    
    public void execute();
    
}
