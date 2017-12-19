using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LoopKata
{
    // Not the real LoopDectector only implemented to avoid Compilation errors
    // The real one is provided by the Kata itself
    class LoopDetector
    {
        public LoopDetector() { }

        public class Node
        {
            public Node() { }
            Node Next = new Node();
            public Node next
            {
                get
                {
                    return Next;
                }
            }
        }
    }

    class Program
    {
        static void Main()
        {

        }

        public static int getLoopSize(LoopDetector.Node startNode)
        {
            // Dictionnary is used to fill in the different nodes the program goes through the loop
            // It helps shorten the process time to verify if the node has already been visited before,
            // because the node itself is use as the hash to access the dictionnary value
            Dictionary<LoopDetector.Node, int> nodes = new Dictionary<LoopDetector.Node, int>();
            bool hasBeenFound = false;
            int currIndex = 0;
            int firstLoopIndex = 0;
            int loopLenght = 0;
            LoopDetector.Node currNode = startNode;

            // Since we have no idea how long the loop is and no there is way to tell it anyhow,
            // While loop is used to go through the loop untill a node in the dictionnary is crossed for the second time
            while (!hasBeenFound)
            {
                // every time the iteration is done, adds a new node the dictionnary and its current position
                // which is going to serve us for the lenght calculation
                nodes.Add(currNode, currIndex++);

                // If the following node is already in the dictionnary, it means that the we have reached the end,
                // We, then, need to stop the iteration has the ending is connected to a previous node making the loop infinite 
                if (nodes.ContainsKey(currNode.next))
                {
                    // Calculate the current distance minus the next Node Index to determine
                    // how long the loop has been from this point to the end
                    nodes.TryGetValue(currNode.next, out firstLoopIndex);
                    loopLenght = currIndex - firstLoopIndex;
                    hasBeenFound = true;
                }
                currNode = currNode.next;
            }
            return loopLenght;
        }
    }
}
