import React, {Component} from 'react';
import withHeader from '../components/header'

class Test1 extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
            <form method="POST" encType="multipart/form-data">
                <input type="file" name="data"/>
            </form>
        );
    }
}

export default withHeader(Test1, "Test1");