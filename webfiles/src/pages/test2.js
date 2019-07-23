import React, {Component} from 'react';
import withHeader from '../components/header'

class Test2 extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
            <div></div>
        );
    }
}

export default withHeader(Test2, "Test2");